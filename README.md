# Tideman
#### Description
In CS50's Introduction to Computer Science (Harvard University Computer Science 50), algorithms are defined to be procedures that map inputs to outputs (however, are correct only if every input maps to a correct output). The specification for what the inputs and the outputs are defined to be correct is a problem; hence, designing and implementing algorithms can solve many real-world problems by abstracting it to input and outputs spaces with relations between these objects. For instance, many elections are being held worldwide for many executive and legistlative officials in first-world nations at present in 2024; hence, writing software to obtain and count the votes can be provide a solution to maintain correctncess and efficency because of the number of votes that are typically casted. Ergo, this problem can be specified as follows where the input space are the votes and the output space are the candidates; however, problems need to specify what a correct relation between inputs and outputs (othwerwise, the problem has no meaning). For this problem of elections, we need to define what method to determine the winner of an election. A common method is plurality elections: every votor gets one vote, and the candidate with the most votes wins the election; however, this can lead to ties amoung candidates for small instances. Another common method is a runoff election: every votor gets one vote, and the candidates with the most votes are send to different rounds of voting iteratively until the candidate with the most votes wins the election. The method of choice that shall be specified for this problem is a Tideman election. 

Tideman elections are ranked-choice voting systems where votors can vote for more than one candidate by ordering the candidates keyed on preference. Next, the computation of the winner is to determine the "Condorcet" winner (if they exist): the candidate that wins in all head-to-head matchup against other candidates. In essences, the procedure makes a directed graph with the votes and finds the "source": the node that has only outward edges i.e. the winner (ignore edges that create cycles i.e. graphs without a source).
#### Specification
tideman.c is the source code file written in C that runs a tideman election on an array of candidates. The procedure main defines the direct computation of the election with a few subprocedure calls to help with the computation. Given that the complexity of the computation scales with the number of candidates, we defined a macro MAX to 9 for limiting the number of candidates in an election. Next, we define a matrix perferences of integers to obtain the number of votors who prefer candidate i over candidate j. Since this is a ranked-pairs election, we define a struct pair where the indices of the winner and loser are recorded. 

main is the procedure to directly compute the winner by making a tree-structure graph (e.g. there exists a source at each iteration) by using the string50 library to obtain the ranked-choice votes and populate the preferences matrix iteratively with each vote. This algorithm calls the following subprocedures to achieve the winner of the election in sequence order:

vote is a subprocedure that directly gets the ranks (ordered sequence of prefered candidates per votor) for each call. 

record_preferences is a subprocedure that updates the preference matrix based upon the ranks per vote.

add_pairs is a subprocedure that sets the candidates into winner and loser pairs.

sort_pairs is a subprocedure that arranges the pairs in decreasing order by strength of victory i.e. larger differences in votes means the stronger victory.

lock_pairs is a subprocedure that constructs the graph with a call to make_cycles (a helper function) that ensure that this pair does not make a cycle and add it to the graph if a cycle is not induced with the insertion of this pair.

print_winner is a subprocedure that return the winner of the election.
