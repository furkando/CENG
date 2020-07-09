import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.transforms as transforms
from torchvision.datasets import CIFAR10

torch.manual_seed(1234)

device = "cuda"
batch_size = 128
num_epochs = 6
ratio = 0.1


class MyModel(nn.Module):
    num_features = 32 * 32 * 3
    num_outputs = 10

    def __init__(self, num_hidden1, num_hidden2):
        super().__init__()
        self.first_flag = num_hidden1
        self.second_flag = num_hidden2

        if num_hidden1 is None:
            self.output = nn.Linear(MyModel.num_features, MyModel.num_outputs)
        elif num_hidden2 is None:
            self.hidden1 = nn.Linear(MyModel.num_features, num_hidden1)
            self.output = nn.Linear(num_hidden1, MyModel.num_outputs)
        else:
            self.hidden1 = nn.Linear(MyModel.num_features, num_hidden1)
            self.hidden2 = nn.Linear(num_hidden1, num_hidden2)
            self.output = nn.Linear(num_hidden2, MyModel.num_outputs)

    def forward(self, x):
        x = torch.flatten(x, 1)

        if self.first_flag is None:
            x = self.output(x)
        elif self.second_flag is None:
            x = F.leaky_relu(self.hidden1(x))
            x = self.output(x)
        else:
            x = F.leaky_relu(self.hidden1(x))
            x = F.leaky_relu(self.hidden2(x))
            x = self.output(x)

        return x


# Test accuracy: 54%
if __name__ == '__main__':
    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    train_set = CIFAR10(root="task2_data", train=True, transform=transform, download=True)
    test_set = CIFAR10(root="task2_data", train=False, transform=transform, download=True)
    train_set, val_set = torch.utils.data.random_split(train_set,
                                                 [int((1 - ratio) * len(train_set)),
                                                  int(ratio * len(train_set))])

    train_loader = torch.utils.data.DataLoader(train_set, batch_size=batch_size, shuffle=True)
    val_loader = torch.utils.data.DataLoader(val_set, batch_size=batch_size)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=batch_size)

    model_no_hidden = MyModel(None, None)
    model_one_hidden = MyModel(1000, None)
    model_two_hidden = MyModel(500, 250)
    model = model_two_hidden.to(device)

    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    my_loss = nn.CrossEntropyLoss()

    for epoch in range(num_epochs):
        sum_loss = 0
        for i, (images, labels) in enumerate(train_loader):
            images = images.to(device)
            labels = labels.to(device)

            optimizer.zero_grad()
            output = model(images)
            loss = my_loss(output, labels)
            loss.backward()
            optimizer.step()
            sum_loss += loss.item()

        with torch.no_grad():
            loss = 0
            for j, (images, labels) in enumerate(val_loader):
                images = images.to(device)
                labels = labels.to(device)

                output = model(images)
                loss += my_loss(output, labels).item()
            print('Epoch [%d] Train Loss: %.4f Val Loss: %.4f' % (epoch+1, sum_loss/i, loss/j))

    with torch.no_grad():
        correct = total = 0
        for images, labels in test_loader:
            images = images.to(device)
            labels = labels.to(device)

            output = model(images)
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)
        print('Percent correct: %.3f %%' % ((100 * correct) / (total + 1)))
