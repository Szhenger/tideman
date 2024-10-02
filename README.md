# Tideman

## Problem to Solve

Given the 2024 election landscape, a fair method of determining winners is crucial, especially one that reflects the preferences of the majority. While many voting systems exist, the most commonly used is the plurality method, where each voter selects one candidate, and the candidate with the most votes wins. However, this method has significant drawbacks.

First, in the event of a tie, the plurality method may require a costly and time-consuming runoff election. Second, it can result in a candidate winning even when the majority of voters do not support them. For example, in an election with three candidates and nine voters, if Candidate 1 receives four votes, Candidate 2 gets three, and Candidate 3 gets two, Candidate 1 wins under the plurality method — despite the fact that the majority (five voters) did not prefer Candidate 1.

A potential alternative is ranked-choice voting, where voters rank candidates by preference (e.g., first, second, third). This system helps ensure that the elected candidate has broader support from the electorate, reducing the likelihood that a polarizing candidate wins with only a minority of the votes. In a ranked-choice voting system, the ballets are counted to find the so-called "Condorcet winner": the person who would have won any head-to-head matchup against another candidate. 

The Tideman voting method (also known as the "ranked pairs" method) is a ranked-choice voting system that guarantees the election of the Condorcet winner if one exists. This repository contains the source code that simulates an election using the Tideman method, demonstrating how ranked pairs can be applied to ensure a fair and representative outcome.

## Background

In general, the Tideman method works by constructing a directed graph data structure of candidates, where an arrow (i.e. edge) from Candidate 1 to Candidate 2 indicates that Candidate 1 wins in a head-to-head match against Candidate 2. The method seeks to identify the source of this graph — defined as the candidate with no incoming edges (i.e., no arrows pointing toward them) — as the winner. However, in some voting configurations, a Condorcet winner (a candidate who wins all head-to-head matches) may not exist. In such cases, the graph may contain cycles, meaning there is no clear source. Indeed, the Tideman method is only valid for input configurations where a source exists.

In addition, when constructing the graph, the Tideman algorithm ensures no cycles are formed, as cycles would prevent the identification of a source (winner). The algorithm begins by adding the strongest edges first—those representing the most decisive victories, where a greater number of voters prefer one candidate over another.

Each edge is added based on the strength of victory (i.e., the margin by which one candidate defeats another), and edges are locked into the graph as long as they do not create a cycle. If adding an edge would result in a cycle, it is ignored to maintain the integrity of the directed acyclic graph (i.e. directed graph with no cycles), ensuring that a valid source can be identified if one exists.

Formally, the Tideman algorithm consists of three procedures:

* Tally: Assuming that the voters have indicated all of their preferences, calculate, for each pair of candidates, who the preferred candidate is and by what margin they are preferred.
* Sort: Sort the pairs of candidates in decending order of strength of victory, where strength of victory is defined to be the margin of voters who prefer the preferred candidate.
* Lock: Starting with the strongest pair, progressively "lock in" each pair to the directed graph, ensuring that locking a pair does not create a cycle. This process continues for all pairs in order, avoiding any updates that would result in a cycle.

## Understanding

Consider the `tideman.c` source code file. 

First, notice the integer-valued matrix `preferences` (represented as a two-dimensional array of integers) that tracks voter preferences between candidates. The integer `preferences[i][j]` represents the number of voters who prefer Candidate i over Candidate j. 

Second, the file also defines another boolean-valued matrix `locked` (again represented as a two-dimensional array of boolean values), which will represent the directed candidate graph, so `locked[i][j]` being `true` represents the existence of an directed edge from Candidate i to Candidate j, meaning Candidate i is locked in as preferred over Candidate j else, there is no edge between Candidate i and Candidate j.

Third, a data structure is defined called `pair` (represented as a struct), used to represent a pair of candidates: each pair includes the winner's candidate index and the loser's candidate index (i.e. adding semantics to the candidate indexes). The candidates are stored in a string-valued vector `candidates` (represented as an array of `string`s), representing the names of the candidates. There is also an array `pairs`, which contains all of the pairs of candidates (for which one is preferred over the other) in the election.

Fourth, the program defines two global variables: `candidate_count` and `pairs_count`, representing the numbers of candidates and pairs in the arrays candidates and pairs, respectively. 

In the main procedure, the program loops through the locked graph and intially sets all the values to false, which indicates our initial graph has no edges in it.

Next, the program loops over all of the votors and gets their preferences in an array called `ranks` (via a call to `vote`) with `ranks[i]` is the index of the candidate who is the `i`th preference for the voter. These ranks are passed into the `record_preference` function, which takes the ranks and updates the `preferences` array.

Last, once all the votes are collected, the pair of candidates are added to the `pairs` array via a call to `add_pairs`, sorted via a call to `sort_pairs`, and locked into the graph via a call to `lock_pairs`. At the end, `print_winner` is called to return the name of the election's winner to the command-line.

## Specification

Start by looking at the `tideman.c` source code file. Then we have:

* `vote` function.
    * Input: Takes arguments `name`, `rank`, and `ranks`. If `name` is a match for a name of a valid candidate, then update the `ranks` array to indicate that the voter has the          candidate ranked as their `rank` preference (with `0` is first preference, `1` is second perference, etc.).
    * Output: Returns `true` if the rank was successfully recorded; else, `false` if the name is not valid or the rank is already specified for another prior candidate.
    * Assume that no two candidates have the same name.
* `record_preferences` function.
    * This function is called once per voter.
    * Input: Takes as argument the `ranks` array (assume the invariant that ranks[i] is the voter's `i`th candidate preference is true).
    * Side Effect: Updates the global `preferences` array with the current voter preferences.
    * Assume that every votor ranks every candidate correctly (i.e. every candidates gets a rank by votor).
* `add_pairs` function.
    * The function adds all pairs of candidates such that one candidate is preferred to the `pairs` array. A pair of candidates who are tied (no one is preferable over the other)        is not addded to the array.
    * The function updates the global `pairs_count` variable to be the number of pairs added.
* `sort_pairs` function.
    * This function sorts the `pairs` array in decreasing order of strength of victory. If multiple pairs share the same strength of victory, then the order does not matter.
* `locked` function.
    * The function makes the `locked` graph, adding all edges in decreasing order of strength; however, only if the edge does not create       a cycle that we calculate via a call to the `makes_cycle` function.
* `makes_cycle` function.
    * The function checks whether an edge makes a cycle in the `locked` graph if added recursively.
* `print_winner` function.
    * This function calculates the source of the `locked` graph and prints the name of the asscociated candidate.  

## Credit and Disclaimer

This problem orignates from [CS50's Introduction to Computer Science 2024 (Computer Science 50)](https://cs50.harvard.edu/x/2024/psets/3/tideman/) at Harvard University with the `string50` header files supporting the string manipulation operations that the `vote` function calls in `get_string` developed by the CS50 Team and any solution here is explicitly for educational purposes only.
