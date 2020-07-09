import torchvision.transforms as transforms
from torchvision.datasets import CIFAR10
import torch
import torch.nn as nn
import torch.nn.functional as F

torch.manual_seed(1234)


class OneLayerModel(nn.Module):
    num_features = 32 * 32 * 3
    num_categories = 10

    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(OneLayerModel.num_features, 750)
        self.fc2 = nn.Linear(750, OneLayerModel.num_categories)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.relu(self.fc1(x))
        x = self.fc2(x)
        return x


class TwoLayerModel(nn.Module):
    num_features = 32 * 32 * 3
    num_categories = 10

    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(OneLayerModel.num_features, 750)
        self.fc2 = nn.Linear(750,  200)
        self.fc3 = nn.Linear(200, OneLayerModel.num_features)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


class ThreeLayerModel(nn.Module):
    num_features = 32 * 32 * 3
    num_categories = 10

    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(OneLayerModel.num_features, 750)
        self.fc2 = nn.Linear(750,  200)
        self.fc3 = nn.Linear(200, 50)
        self.fc4 = nn.Linear(50, OneLayerModel.num_categories)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.selu(self.fc3(x))
        x = self.fc4(x)
        return x


def net(layer_number, num_epochs, learning_rate, batch_size, device="cpu"):
    device = device

    ratio = 0.1
    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
    train_set = CIFAR10(root="task2_data", train=True,
                        transform=transform, download=True)
    test_set = CIFAR10(root="task2_data", train=False,
                       transform=transform, download=True)
    train_set, val_set = torch.utils.data.random_split(
        train_set, [int((1 - ratio) * len(train_set)), int(ratio * len(train_set))])

    train_loader = torch.utils.data.DataLoader(
        train_set, batch_size=batch_size, shuffle=True)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=batch_size)

    if layer_number == 1:
        model = OneLayerModel().to(device)
    elif layer_number == 2:
        model = TwoLayerModel().to(device)
    else:
        model = ThreeLayerModel().to(device)

    my_loss = nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)

    for epoch in range(num_epochs):
        sum_loss = 0
        for i, (images, labels) in enumerate(train_loader):
            images = images.to(device)
            labels = labels.to(device)

            output = model(images)
            loss = my_loss(output, labels)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            sum_loss += loss.item()

        print('Epoch [%d] Train Loss: %.4f' % (epoch+1, sum_loss/i))

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


print("Training with best parameters for one layer:")
net(1, 12, 0.1, 1000)
print("Training with best parameters for two layer:")
net(2, 12, 0.1, 1000)
print("Training with best parameters for three layer:")
net(3, 11, 0.1, 1000)
