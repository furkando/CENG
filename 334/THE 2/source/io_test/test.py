import os

files = os.listdir("inputs")

for file in files:
	os.system("./simulator < inputs/" + file + " > outputs/out_" + file.split('_')[1])