# Student Name: Mehmet Baris Yaman
# Student Number: 150130136
# The code uses the functions implemented in SimpleAI Library. The library can be found on the link below.
# http://simpleai.readthedocs.io/en/latest/_modules/simpleai/search/csp.html#min_conflicts
from simpleai.search import CspProblem
from simpleai.search import convert_to_binary
from simpleai.search import min_conflicts
import math

variables = range(0, 100)
selections = ['Black', 'White']


domains= {}
inputArray = []

for i in range(len(variables)):
	domains[variables[i]] = selections

# This part converts the line of input files into an integer array namely 'inputArray'
# For example the line '2 1 2' is converted 222 and added to the inputArray

file = open('example_input.txt', 'r')
inputArray = file.readlines()
for i in range(0, 22):
	wrongInput = inputArray[i]
	numberedInput = wrongInput.split('\r')
	inputArray[i] = numberedInput[0]
for i in range(0, 22):
	wrongInput = inputArray[i]
	numberedInput = wrongInput.split(' ')
	total = 0
	counter = 1
	for j in reversed(range(0, len(numberedInput))):
		total = total + counter * int(numberedInput[j])
		counter = counter *10
	inputArray[i] = total
print 'Input Array:'
print inputArray
print 'Min conflict algorithm starts'


def check_constraint(values, number, name):
	#Constraints with respect to a column or a row are taken as:
	#For 4 digit ones such as 2311 firstConstraint=2, secondConstraint=3, thirdConstraint=1, fourthConstraint=1 (number is 2311)
	#For 3 digit ones such as 231 firstConstraint=2, secondConstraint=3, thirdConstraint=1 (number is 231)
	#For 2 digit ones such as 23 firstConstraint=2, secondConstraint=3 (number is 23)
	#For 1 digit ones such as 2 firstConstraint=2 (number is 2)
	secondConstraint = None
	thirdConstraint = None
	fourthConstraint = None
	if number > 1000:
		firstConstraint = math.floor(number/1000)
		secondConstraint = math.floor((number - firstConstraint * 1000)/ 100)
		thirdConstraint = math.floor((number - firstConstraint * 1000 - secondConstraint * 100)/ 10)
		fourthConstraint = number % 10
	elif number > 100:
		firstConstraint = math.floor(number/100)
		secondConstraint = math.floor((number - firstConstraint * 100)/ 10)
		thirdConstraint = number % 10
		fourthConstraint = 0
	elif number>10 :
		firstConstraint = number / 10
		secondConstraint = number %10
		thirdConstraint = 0
		fourthConstraint = 0
	elif number >0:
		firstConstraint = number
		secondConstraint = 0
		thirdConstraint = 0
		fourthConstraint = 0
	else:
		print 'Constraint does not fit'
		return False
	#name value holds the name of the constraint
	#firstBlackCounter counts the number of black values visualized at first
	#secondBlackCounter counts the number of black values visualized at second
	#thirdBlackCounter counts the number of black values visualized at third
	#fourthBlackCounter counts the number of black values visualized at fourth
	#firstBlack is only True when we see the first black values
	#secondBlack is only True when we see the second black values
	#thirdBlack is only True when we see the third black values
	#fourthBlack is only True when we see the fourth black values
	firstBlackCounter = 0
	secondBlackCounter = 0
	thirdBlackCounter = 0
	fourthBlackCounter = 0
	firstBlack = False
	secondBlack = False
	thirdBlack = False
	fourthBlack = False
	if number > 1000:
		for i in range(len(values)):
			if fourthBlack:
				if values[i] == 'Black':
					fourthBlackCounter = fourthBlackCounter + 1
				elif values[i] == 'White':
					fourthBlack = False
				else: 
					return False
			elif thirdBlack:
				if values[i] == 'Black':
					thirdBlackCounter = thirdBlackCounter + 1
				elif values[i] == 'White':
					thirdBlack = False
				else: 
					return False
			elif secondBlack:
				if values[i] == 'Black':
					secondBlackCounter = secondBlackCounter + 1
				elif values[i] == 'White':
					secondBlack = False
				else: 
					return False
			elif firstBlack:
				if values[i] == 'Black':
					firstBlackCounter = firstBlackCounter + 1
				elif values[i] == 'White':
					firstBlack = False
				else: 
					return False
			elif values[i] == 'White':
				continue
			elif values[i]== 'Black':
				if firstBlackCounter == 0:
					firstBlack = True
					firstBlackCounter = firstBlackCounter + 1
				elif secondBlackCounter == 0:
					secondBlack = True
					secondBlackCounter = secondBlackCounter + 1
				elif thirdBlackCounter == 0:
					thirdBlack = True
					thirdBlackCounter = thirdBlackCounter + 1
				elif fourthBlackCounter == 0:
					fourthBlack = True
					fourthBlackCounter = fourthBlackCounter + 1
				else:
					return False
			else:
				return False
	elif number > 100:
		for i in range(len(values)):
			if thirdBlack:
				if values[i] == 'Black':
					thirdBlackCounter = thirdBlackCounter + 1
				elif values[i] == 'White':
					thirdBlack = False
				else: 
					return False
			elif secondBlack:
				if values[i] == 'Black':
					secondBlackCounter = secondBlackCounter + 1
				elif values[i] == 'White':
					secondBlack = False
				else: 
					return False
			elif firstBlack:
				if values[i] == 'Black':
					firstBlackCounter = firstBlackCounter + 1
				elif values[i] == 'White':
					firstBlack = False
				else: 
					return False
			elif values[i] == 'White':
				continue
			elif values[i]== 'Black':
				if firstBlackCounter == 0:
					firstBlack = True
					firstBlackCounter = firstBlackCounter + 1
				elif secondBlackCounter == 0:
					secondBlack = True
					secondBlackCounter = secondBlackCounter + 1
				elif thirdBlackCounter == 0:
					thirdBlack = True
					thirdBlackCounter = thirdBlackCounter + 1
				else:
					return False
			else:
				return False
	elif number > 10:
		for i in range(len(values)):
			if secondBlack:
				if values[i] == 'Black':
					secondBlackCounter = secondBlackCounter + 1
				elif values[i] == 'White':
					secondBlack = False
				else: 
					return False
			elif firstBlack:
				if values[i] == 'Black':
					firstBlackCounter = firstBlackCounter + 1
				elif values[i] == 'White':
					firstBlack = False
				else: 
					return False
			elif values[i] == 'White':
				continue
			elif values[i]== 'Black':
				if firstBlackCounter == 0:
					firstBlack = True
					firstBlackCounter = firstBlackCounter + 1
				elif secondBlackCounter == 0:
					secondBlack = True
					secondBlackCounter = secondBlackCounter + 1
				else: 
					return False
			else:
				return False
	else:
		for i in range(len(values)):
			if firstBlack:
				if values[i] == 'Black':
					firstBlackCounter = firstBlackCounter + 1
				elif values[i] == 'White':
					firstBlack = False
				else: 
					return False
			elif values[i] == 'White':
				continue
			elif values[i]== 'Black':
				if firstBlackCounter == 0:
					firstBlack = True
					firstBlackCounter = firstBlackCounter + 1
				else: 
					return False
			else:
				return False
	# For the constraints that are related to fourthConstraint or thirdConstraint are always 0 and equal to the counters(firstBlackCounter etc.)
	if firstConstraint == firstBlackCounter and secondConstraint == secondBlackCounter and thirdConstraint == thirdBlackCounter and fourthConstraint== fourthBlackCounter:
		return True
	else:
		return False	

# inputArray[0] is passed since it is 10 showing to number of rows

def const_first_row(variables, values):
	return check_constraint(values, inputArray[1], 'First Row Constraint')

def const_second_row(variables, values):
	return check_constraint(values, inputArray[2], 'Second Row Constraint')

def const_third_row(variables, values):
	return check_constraint(values, inputArray[3], 'Third Row Constraint')

def const_fourth_row(variables, values):
	return check_constraint(values, inputArray[4], 'Fourth Row Constraint')

def const_fifth_row(variables, values):
	return check_constraint(values, inputArray[5], 'Fifth Row Constraint')

def const_sixth_row(variables, values):
	return check_constraint(values, inputArray[6], 'Sixth Row Constraint')

def const_seventh_row(variables, values):
	return check_constraint(values, inputArray[7], 'Seventh Row Constraint')

def const_eight_row(variables, values):
	return check_constraint(values, inputArray[8], 'Eight Row Constraint')

def const_nineth_row(variables, values):
	return check_constraint(values, inputArray[9], 'Nineth Row Constraint')

def const_tenth_row(variables, values):
	return check_constraint(values, inputArray[10], 'Tenth Row Constraint')

# inputArray[11] is passed since it is 10 showing to number of columns

def const_first_column(variables, values):
	return check_constraint(values, inputArray[12], 'First Column Constraint')

def const_second_column(variables, values):
	return check_constraint(values, inputArray[13], 'Second Column Constraint')

def const_third_column(variables, values):
	return check_constraint(values, inputArray[14], 'Third Column Constraint')

def const_fourth_column(variables, values):
	return check_constraint(values, inputArray[15], 'Fourth Column Constraint')

def const_fifth_column(variables, values):
	return check_constraint(values, inputArray[16], 'Fifth Column Constraint')

def const_sixth_column(variables, values):
	return check_constraint(values, inputArray[17], 'Sixth Column Constraint')

def const_seventh_column(variables, values):
	return check_constraint(values, inputArray[18], 'Seventh Column Constraint')

def const_eight_column(variables, values):
	return check_constraint(values, inputArray[19], 'Eight Column Constraint')

def const_nineth_column(variables, values):
	return check_constraint(values, inputArray[20], 'Nineth Column Constraint')

def const_tenth_column(variables, values):
	return check_constraint(values, inputArray[21], 'Tenth Column Constraint')




constraints = [
	((variables[0:10]), const_first_row),
	((variables[10:20]), const_second_row),
	((variables[20:30]), const_third_row),
	((variables[30:40]), const_fourth_row),
	((variables[40:50]), const_fifth_row),
	((variables[50:60]), const_sixth_row),
	((variables[60:70]), const_seventh_row),
	((variables[70:80]), const_eight_row),
	((variables[80:90]), const_nineth_row),
	((variables[90:100]), const_tenth_row),
	(list(variables[i] for i in range(0, 100, 10)), const_first_column),
	(list(variables[i] for i in range(1, 100, 10)), const_second_column),
	(list(variables[i] for i in range(2, 100, 10)), const_third_column),
	(list(variables[i] for i in range(3, 100, 10)), const_fourth_column),
	(list(variables[i] for i in range(4, 100, 10)), const_fifth_column),
	(list(variables[i] for i in range(5, 100, 10)), const_sixth_column),
	(list(variables[i] for i in range(6, 100, 10)), const_seventh_column),
	(list(variables[i] for i in range(7, 100, 10)), const_eight_column),
	(list(variables[i] for i in range(8, 100, 10)), const_nineth_column),
	(list(variables[i] for i in range(9, 100, 10)), const_tenth_column),
]

#In order to decrease the run time of the algorithm
variables, domains, constraints = convert_to_binary(variables, domains, constraints)
	
my_problem = CspProblem(variables, domains, constraints)

result = min_conflicts(my_problem)

for k, v in result.items():
	print(k, v)


