# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

"""
    Student Name: Mehmet Baris Yaman
    Student ID: 150130136
"""

import util
from Queue import *
from simpleai.search import astar as AstarAlgorithm





class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:
    

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    util.raiseNotDefined()

    Start: (35, 1)
    Is the start a goal? False
    Start's successors: [((35, 2), 'North', 1), ((34, 1), 'West', 1)]

    "*** YOUR CODE HERE ***"
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    n = Directions.NORTH
    e = Directions.EAST
    stack = []
    aka = problem.getSuccessors(problem.getStartState())
    result = []
    """Insert first two successors into Stack"""
    for x in aka:
        listX = list(x)
        listX.insert(3, x[1])
        tupleX = tuple(listX)
        stack.append(tupleX)
    while stack:
        selectedNode = stack.pop()
        if problem.isGoalState(selectedNode[0]):
            print "Finish: ", selectedNode
            finishNode = selectedNode
            break
        temp = problem.getSuccessors(selectedNode[0])
        """temp is a tuple, for backtracking I am adding direction strings into the tuple
            To achieve that, I first convert the tuple to a list"""
        for x in temp:
            listX = list(x)
            for i in selectedNode[3:]:
                listX.append(i)
            listX.append(x[1])
            tupleX = tuple(listX)
            stack.append(tupleX)
    """Append directions with respect to direction strings"""
    finishNodeList = list(finishNode)
    for a in finishNodeList[3:]:
        if a == 'North':
            result.append(n)
        elif a == 'South':
            result.append(s)
        elif a== 'West':
            result.append(w)
        elif a== 'East':
            result.append(e)
    return result

def breadthFirstSearch(problem, graph_search=False, viewer=None):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    """
    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    """util.raiseNotDefined()"""
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    n = Directions.NORTH
    e = Directions.EAST
    queue = Queue(maxsize=0)
    aka = problem.getSuccessors(problem.getStartState())
    result = []
    """Insert first two successors into Queue"""
    for x in aka:
        listX = list(x)
        listX.insert(3, x[1])
        tupleX = tuple(listX)
        queue.put(tupleX)
    while queue:
        selectedNode = queue.get()
        if problem.isGoalState(selectedNode[0]):
            print "Finish: ", selectedNode
            finishNode = selectedNode
            break
        temp = problem.getSuccessors(selectedNode[0])
        """temp is a tuple, for backtracking I am adding direction strings into the tuple
            To achieve that, I first convert the tuple to a list"""
        for x in temp:
            listX = list(x)
            for i in selectedNode[3:]:
                listX.append(i)
            listX.append(x[1])
            tupleX = tuple(listX)
            queue.put(tupleX)
    finishNodeList = list(finishNode)
    """Append directions with respect to direction strings"""
    for a in finishNodeList[3:]:
        if a == 'North':
            result.append(n)
        elif a == 'South':
            result.append(s)
        elif a== 'West':
            result.append(w)
        elif a== 'East':
            result.append(e)
    return result





def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 1

"""Implemented according to Simple AI Library. Can be found on this link:
    https://github.com/simpleai-team/simpleai """
def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    n = Directions.NORTH
    e = Directions.EAST
    class BigMazeProblem(SearchProblem):
        """isNull is for determining which heuristic will be used"""
        isNull = True
        numberOfNodesGenerated=0
        numberOfNodesExplored=0
        def __init__(self, state, heuristic):
            self.initial_state = state
            if heuristic == nullHeuristic:
                self.isNull = True
            else:
                self.isNull = False
        
        def actions(self, state):
            if not problem.isGoalState(state):
                """Take the direction of the closest successor"""
                self.numberOfNodesExplored = self.numberOfNodesExplored+1
                return list(map(lambda x: x[1], problem.getSuccessors(state)))
    
        def is_goal(self, state):
            return problem.isGoalState(state)

        def result(self, state, action):
            if(action=='North'):
                return (state[0], state[1]+1)
            elif(action=='South'):
                return (state[0], state[1]-1)
            elif(action=='East'):
                return (state[0]+1, state[1])
            elif(action=='West'):
                return (state[0]-1, state[1])
    
        def heuristic(self, state):
            # how far are we from the goal?
            self.numberOfNodesGenerated = self.numberOfNodesGenerated+1
            if self.isNull:
                return 1
            else:
                return util.manhattanDistance(state, (1,1))
    
        def cost(self, state, action, state2):
            return 1

    astarProblem = BigMazeProblem(state= problem.getStartState(), heuristic=heuristic)
    result = AstarAlgorithm(astarProblem, graph_search=True)
    
    print(result.path())
    print "Number of nodes generated: ",astarProblem.numberOfNodesGenerated
    print "Max number of nodes kept in memory: ", astarProblem.numberOfNodesGenerated + astarProblem.numberOfNodesExplored
    resultArray = []
    for i in result.path()[1:]:
        if i[0] == 'North':
            resultArray.append(n)
        elif i[0] == 'South':
            resultArray.append(s)
        elif i[0] == 'West':
            resultArray.append(w)
        elif i[0] == 'East':
            resultArray.append(e)
    return resultArray

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
