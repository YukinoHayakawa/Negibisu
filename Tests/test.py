import glob, os
import subprocess

# https://stackoverflow.com/questions/20421187/python-save-the-output-of-a-shell-command-into-a-text-file
for file in glob.glob("*.negi"):
    print(file)
    output = open(file + ".out", "w")
    subprocess.Popen(["../../../../x64/Debug/Negibisu", file], stdout=output)
