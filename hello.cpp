#include <iostream>
using namespace std;
int main() {
  struct foo_t {
    int x[100];
    int var1;
    int y[10];
    } foo;
  int var2;
  long i;
  int *p, *q;
  short int *s;
  long int *l;
  struct foo_t bar[50];

  for (i=0; i<100; i++) foo.x[i]=300+i;
  for (i=0; i<10; i++)  foo.y[i]=800+i;
  foo.var1 = 440;

  cout << sizeof(*s) << "\n";
  cout << sizeof(*p) << "\n";
  cout << sizeof(*l) << "\n";
  q = (int *) &foo;    cout << q << "\n";
  p=&(foo.x[5]);       cout << *p << "\n";
  // POINT 1
  q = (int *) &var2;   cout << q << "\n";
  q = p+16;            cout << *q << "\n";
  i = ((long) p) + 16;
  q = (int *) i;       cout << *q << "\n";
  s = (short *) i;     cout << *s << "\n";
  l = (long *) i;      cout << *l << "\n";
  q = p+95;            cout << *q << "\n";  
  i = ((long) p) + 17;
  q = (int *) i;       cout << *q << "\n";  // EXPLAIN
  //q = p + _______;     cout << *q << "\n";
  //q = (int *) (((long) p) + _______);  cout << *q << "\n";
  p = (int *) bar;
  //*(p + _______) = 947; cout << bar[8].var1 << "\n";
}