import torch
import torch.nn as nn
import torchvision.transforms as transforms
from torchvision.datasets import MNIST
from task3 import MyModel  # import the model

torch.manual_seed(1234)

device = "cuda"
batch_size = 1
num_epochs = 1

# Test accuracy: 97%
if __name__ == '__main__':
    rand_transform = transforms.Compose([transforms.RandomChoice([
        transforms.RandomCrop(26), transforms.RandomCrop(27),
        transforms.Pad(2), transforms.Pad(1)
    ]), transforms.ToTensor()])

    train_data = MNIST(root='task3_data', train=True, transform=rand_transform, download=True)
    test_data = MNIST(root='task3_data', train=False, transform=rand_transform, download=True)

    train_loader = torch.utils.data.DataLoader(dataset=train_data, batch_size=batch_size, shuffle=True)
    test_loader = torch.utils.data.DataLoader(dataset=test_data, batch_size=batch_size)

    model = MyModel().to(device)
    loss_fun = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=1.e-3)

    for epoch in range(num_epochs):
        sum_loss = 0
        for i, (images, labels) in enumerate(train_loader):
            images = images.to(device)
            labels = labels.to(device)

            optimizer.zero_grad()
            output = model(images)
            loss = loss_fun(output, labels)
            loss.backward()
            optimizer.step()
            sum_loss += loss.item()
            if (i + 1) % 5000 == 0:
                print('Epoch [%d/%d], Step [%d/%d], Loss: %.4f' % (epoch + 1, num_epochs, i + 1,
                                                                   len(train_data) // batch_size, loss.item()))
        print('Epoch [%d] Training Loss: %.4f' % (epoch + 1, sum_loss / i))

    with torch.no_grad():
        correct = total = 0
        for images, labels in test_loader:
            images = images.to(device)
            labels = labels.to(device)

            out = model(images)
            _, predicted_labels = torch.max(out, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)
        print('Percent correct: %.3f %%' % ((100 * correct) / (total + 1)))
