import os
import sys

from Testcase import Testcase
from Miner import Miner
from Smelter import Smelter
from Foundry import Foundry
from Transporter import Transporter
from Constants import *
class Grader(object):
    def __init__(self, studentID, grade_file=None):
        self.studentID = studentID
        self.grade_file = grade_file
        self.testcases = []

    def create_testcases(self):
        for i in xrange(20):
            t = Testcase(i+1)

            if i == 0:
                t += Miner(0, 100, 20, 10)
                t += Transporter(100)
                t += Smelter(0, 100, 20)
            elif i == 1:
                t += Miner(0, 100, 20, 200)
                t += Transporter(100)
                t += Smelter(0, 100, 20)
            elif i == 2:
                t += Miner(1, 100, 20, 200)
                t += Transporter(50)
                t += Smelter(1, 100, 20)
            elif i == 3:
                t += Miner(0, 100, 20, 200)
                t += Miner(2, 100, 20, 200)
                t += Transporter(50)
                t += Foundry(100, 20)
            elif i == 4:
                t += Miner(0, 100, 20, 200)
                t += Miner(2, 100, 20, 200)
                t += Transporter(50)
                t += Foundry(50, 20)
            elif i == 5:
                t += Miner(0, 100, 20, 200)
                t += Miner(1, 100, 20, 200)
                t += Miner(2, 100, 20, 200)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Foundry(100, 20)
            elif i == 6:
                t += Miner(1, 100, 20, 400)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
            elif i == 7:
                t += Miner(0, 100, 20, 400)
                t += Miner(1, 100, 20, 400)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(0, 100, 20)
                t += Smelter(1, 100, 20)
            elif i == 8:
                t += Miner(0, 100, 20, 200)
                t += Miner(2, 100, 20, 200)
                t += Transporter(100)
                t += Transporter(100)
                t += Foundry(100, 20)
            elif i == 9:
                t += Miner(0, 100, 20, 600)
                t += Miner(2, 100, 20, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
            elif i == 10:
                t += Miner(0, 50, 20, 600)
                t += Miner(2, 50, 20, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
            elif i == 11:
                t += Miner(1, 100, 40, 600)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Smelter(1, 100, 20)
            elif i == 12:
                t += Miner(1, 100, 40, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Smelter(1, 100, 20)
            elif i == 12:
                t += Miner(0, 100, 40, 600)
                t += Miner(1, 100, 40, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(0, 100, 20)
                t += Smelter(1, 100, 20)
                t += Smelter(1, 100, 20)
            elif i == 13:
                t += Miner(1, 100, 20, 200)
                t += Miner(1, 100, 20, 200)
                t += Transporter(50)
                t += Smelter(1, 100, 20)
            elif i == 14:
                t += Miner(0, 100, 20, 400)
                t += Miner(2, 100, 20, 400)
                t += Transporter(100)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
            elif i == 15:
                t += Miner(0, 100, 20, 400)
                t += Miner(1, 100, 20, 400)
                t += Miner(2, 100, 20, 400)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Foundry(100, 20)
            elif i == 16:
                t += Miner(0, 100, 20, 400)
                t += Miner(1, 100, 20, 400)
                t += Miner(2, 100, 20, 400)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Foundry(100, 20)
            elif i == 17:
                t += Miner(0, 100, 20, 600)
                t += Miner(1, 100, 20, 600)
                t += Miner(2, 100, 20, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(1, 100, 20)
                t += Smelter(1, 100, 20)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
            elif i == 18:
                t += Miner(0, 100, 10, 1200)
                t += Miner(0, 100, 20, 1200)
                t += Miner(0, 100, 30, 1200)
                t += Miner(1, 100, 10, 600)
                t += Miner(1, 100, 20, 600)
                t += Miner(1, 100, 30, 600)
                t += Miner(2, 100, 10, 600)
                t += Miner(2, 100, 20, 600)
                t += Miner(2, 100, 30, 600)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(100)
                t += Smelter(0, 100, 20)
                t += Smelter(0, 100, 20)
                t += Smelter(1, 100, 20)
                t += Smelter(1, 100, 20)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
                t += Foundry(100, 20)
            elif i == 19:
                t += Miner(0, 200, 10, 3000)
                t += Miner(0, 100, 20, 3000)
                t += Miner(0, 50,  30, 3000)
                t += Miner(1, 400, 10, 2000)
                t += Miner(1, 200, 20, 2400)
                t += Miner(1, 100, 30, 1200)
                t += Miner(2, 200, 10, 1200)
                t += Miner(2, 100, 20, 2000)
                t += Miner(2, 50, 30, 1800)
                t += Transporter(50)
                t += Transporter(50)
                t += Transporter(100)
                t += Transporter(100)
                t += Transporter(200)
                t += Transporter(200)
                t += Transporter(400)
                t += Transporter(400)
                t += Transporter(800)
                t += Transporter(800)
                t += Transporter(1000)
                t += Smelter(0, 100, 30)
                t += Smelter(0, 100, 10)
                t += Smelter(0, 100, 20)
                t += Smelter(0, 100, 70)
                t += Smelter(1, 100, 40)
                t += Smelter(1, 100, 60)
                t += Smelter(1, 100, 50)
                t += Smelter(1, 100, 30)
                t += Foundry(100, 50)
                t += Foundry(100, 20)
                t += Foundry(100, 40)
                t += Foundry(100, 30)
            self.testcases.append(t)


    def grade(self):
        if self.grade_file is not None:
            grades = open(self.grade_file, "a")
        else:
            grades = None
        student_output = open("results.txt", "w")
        try:
            os.makedirs("inputs")
        except OSError:
            pass
        try:
            os.makedirs("outputs")
        except OSError:
            pass
        count = 0
        student_output.write("Results for student %s.\n" % self.studentID)
        for testcase in self.testcases:
            input = open("inputs/testcase%d.txt" % testcase.ID, "w")
            output = open("outputs/testcase%d.txt" % testcase.ID, "w")

            try:
                r, o = testcase.execute(input, output)
            except Exception as e:
                r, o = False, "Testcase%d: Failed.\nError: %s\n" % (self.testcases.index(testcase), str(e))

            if r:
                if testcase.is_finished():
                    count += 1.0
                else:
                    o = "Testcase%d: Failed.\n Agents is not finished.\n" % self.testcases.index(testcase)

            student_output.write(o)

            input.close()
            output.close()

        grade = (count / len(self.testcases)) * 100.0

        student_output.write("Grade: %f\n" % grade)
        if grades is not None:
            grades.write("%s %.2f\n" % (self.studentID, grade))
        else:
            print "Grade: %.2f" % grade


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "<usage> - Grader <student_id> <optional student_grade_file>"

    grader = None
    if len(sys.argv) < 3:
        grader = Grader(sys.argv[1])
    else:
        grader = Grader(sys.argv[1], sys.argv[2])

    grader.create_testcases()
    grader.grade()
