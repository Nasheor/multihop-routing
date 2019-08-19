# Multihop-Routing
Program that constructs and maintains a routing tree to demonstrate __Many to one Routing__. 

## Many to one Routing
1. Construct a tree.
2. The cost (routing metric) is a value reflecting the effort required to deliver the packets along the path.
3. Use hop countThe sink is the root of the tree. It initiates the process by sending a beacon with h=0
4. When a non-root node receives a beacon with a metric h that is better than its current one, it
5. Selects the source of the beacon as the new parent 
6. Sets its own metric to h+1
7. Broadcasts a packet with h+1 as the me


## Periodic tree update
From time to time the routes are refreshed 

