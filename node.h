#ifndef NODE_H_
#define NODE_H_

struct node_t {
  int weight;
  int c;
  struct node_t * left;
  struct node_t * right;
};

#endif // NODE_H_
