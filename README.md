# Ketu

TODO
  - propagate annealing messages to locally formed nodes

## Notes
 - As nodes onverge to formation, nearest nodes are likely already in formation so we need a better way to fetch nearest unformed nodes.
   - Or fetch all the nodes in world sorted by distance if it is realistic
 - updating a nodes neighbors should not change formation slot already assigned to a node
   - how to compare formation slots?
     - enum for each slot
   - how to do it when formation is moving while changing??