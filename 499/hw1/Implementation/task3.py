import torch
import torch.nn as nn
import torch.nn.functional as F


class MyModel(nn.Module):
  num_channels = 1
  num_outputs = 10

  def __init__(self):
    super().__init__()

    self.conv1 = nn.Conv2d(MyModel.num_channels, 25, 5)
    self.conv2 = nn.Conv2d(25, 50, 5)
    self.gap = nn.AdaptiveAvgPool2d(4)
    self.fc1 = nn.Linear(4 * 4 * 50, 400)
    self.fc2 = nn.Linear(400, MyModel.num_outputs)

  def forward(self, x):
    x = F.relu(self.conv1(x))
    x = F.relu(self.conv2(x))
    x = self.gap(x)
    x = F.relu(self.fc1(torch.flatten(x, 1)))
    x = self.fc2(x)

    return x
