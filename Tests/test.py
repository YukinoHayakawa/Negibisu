import glob, os
import subprocess
import sys

# https://stackoverflow.com/questions/20421187/python-save-the-output-of-a-shell-command-into-a-text-file
for file in glob.glob("*.negi"):
    test_case_name = os.path.splitext(file)[0]
    sys.stdout.write("Test: " + test_case_name)

    out_name = test_case_name + ".out"
    diff_name = test_case_name + ".diff"
    cmp_name = test_case_name + ".cmp"

    with open(out_name, "w", newline='\n') as test_output:
        subprocess.Popen(
            ["../../../../x64/Debug/Negibisu", file],
            stdout=test_output
        ).wait()

    with open(diff_name, "w", newline='\n') as diff_output:
        subprocess.Popen(
            ["diff", '--strip-trailing-cr', cmp_name, out_name],
            stdout=diff_output
        ).wait()

    if os.stat(diff_name).st_size > 0:
        print(" - failed")
    else:
        os.remove(diff_name)
        print(" - passed")
