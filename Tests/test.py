import glob, os
import subprocess
import sys

# https://stackoverflow.com/questions/20421187/python-save-the-output-of-a-shell-command-into-a-text-file
for file in glob.glob("*.negi"):
    test_case_name = os.path.splitext(file)[0]
    sys.stdout.write("Test: " + test_case_name)

    out_name = test_case_name + ".out"
    diff_name = test_case_name + ".diff"

    with open(out_name, "wb") as test_output:
        output = subprocess.check_output(
            ["../../../../x64/Debug/Negibisu", file]
        )
        test_output.write(output.replace(b'\r\n', b'\n'))

    if not subprocess.check_output(["git", "ls-files", out_name]):
        print(" - failed (no expected output)")
        continue

    with open(diff_name, "wb") as diff_output:
        subprocess.Popen(
            # compare with index
            ["git", "diff", "--ignore-cr-at-eol", "--", out_name],
            stdout=diff_output
        ).wait()

    if os.stat(diff_name).st_size > 0:
        print(" - failed")
    else:
        os.remove(diff_name)
        print(" - passed")
