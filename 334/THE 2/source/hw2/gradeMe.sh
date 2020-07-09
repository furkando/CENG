studentID=$1
if [ -z $studentID ]; then
    echo "You should give your studentID as a first argument. Example: "
    echo "./gradeMe.sh eXXXXXXX"
    exit
fi
echo "Compiling your code with 'make all'. If this step fails, you will not be correctly graded."
make all
if [ -f simulator ]; then
    python Grader.py $studentID
else
    echo "After compile there should be executable named simulator."
fi
