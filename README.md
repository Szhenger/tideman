# Tideman

## Problem to Solve

Given the 2024 election landscape, a fair method of determining winners is crucial, especially one that reflects the preferences of the majority. While many voting systems exist, the most commonly used is the plurality method, where each voter selects one candidate, and the candidate with the most votes wins. However, this method has significant drawbacks.

First, in the event of a tie, the plurality method may require a costly and time-consuming runoff election. Second, it can result in a candidate winning even when the majority of voters do not support them. For example, in an election with three candidates and nine voters, if Candidate 1 receives four votes, Candidate 2 gets three, and Candidate 3 gets two, Candidate 1 wins under the plurality method — despite the fact that the majority (five voters) did not prefer Candidate 1.

A potential alternative is ranked-choice voting, where voters rank candidates by preference (e.g., first, second, third). This system helps ensure that the elected candidate has broader support from the electorate, reducing the likelihood that a polarizing candidate wins with only a minority of the votes. In a ranked-choice voting system, the ballets are counted to find the so-called "Condorcet winner": the person who would have won any head-to-head matchup against another candidate. 

The Tideman voting method (also known as the "ranked pairs" method) is a ranked-choice voting system that guarantees the election of the Condorcet winner if one exists. This repository contains the source code that simulates an election using the Tideman method, demonstrating how ranked pairs can be applied to ensure a fair and representative outcome.

## Background

In general, the Tideman method works by constructing a directed graph data structure of candidates, where an arrow (i.e. edge) from Candidate 1 to Candidate 2 indicates that Candidate 1 wins in a head-to-head match against Candidate 2. The method seeks to identify the source of this graph — defined as the candidate with no incoming edges (i.e., no arrows pointing toward them) — as the winner. However, in some voting configurations, a Condorcet winner (a candidate who wins all head-to-head matches) may not exist. In such cases, the graph may contain cycles, meaning there is no clear source. Indeed, the Tideman method is only valid for input configurations where a source exists.
## Understanding

TODO

## Specification

TODO
