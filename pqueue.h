/////////////////////////////////// pqueue .h /////////////////////////
#ifndef PQUEUE_H
#define PQUEUE_H // MIN Queue

#include <cstdlib>
#include <iosfwd>
#include <vector>
#include <map>

using std::map;
using std::vector;

template <class J_Pair>

class pqueue
{

  public:

  typedef typename J_Pair::mod_mon sig;

  ~pqueue ()
  {
    while (!array.empty ())
      {
        delete index [array.back ()] ;
        array.pop_back ();
      }
  }

  void enqueue (const J_Pair& j)
  {
    const sig& s = j.uPart ();
    J_Pair* ptr = index[s];
    if (!ptr)
      {
        index[s] = new J_Pair (j);
        array.push_back (s);
        bubble_up (array.size ());
        return;
      }
    ////////////////////// insert replacement policy here ///////////
    // if ( false ) { // P1
    // if ( true ) { // P2
    // if ( ptr−>idx () > j . idx ()) { // P3
    // if ( ptr−>idx () < j . idx ()) { // P4
    if (ptr->vMonomial() > j.vMonomial ())   // P5 Winner
      {
        // if ( ptr−>vMonomial() < j . vMonomial ()) { // P6
        // if ( ptr−>theScale () > j . theScale ()) { // P7
        // if ( ptr−>theScale () < j . theScale ()) { // P8
        // if ( ptr−>tc () < j . tc ()) { // P9
        // if ( ptr−>tc () > j . tc ()) { //P10
        delete ptr;
        index [s] = new J_Pair (j);
      }
  }

  J_Pair dequeue ()
  {
    sig s = array[0] ;
    J_Pair answer = *index[s];
    delete index[s];
    index.erase (s);
    array[0] = array.back ();
    array.pop_back ();
    bubble_down ();
    return answer;
  }

  int size () const
  {
    return array.size ();
  }

  bool empty() const
  {
    return array.empty ();
  }

  void reHeapify ()   // call whenever our term ordering changes (g1/g2)
  {
    for (int i = array.size (); i > 0; i--)
      bubble_down (i);
  }

  private :

  void bubble_up (int start_pos)   // receives 1−based indexing
  {
    if (start_pos > 1 && array[start_pos - 1] < array[start_pos/2 - 1])
      {
        sig temp = array[start_pos - 1];
        array[start_pos - 1] = array[start_pos/2 - 1];
        array[start_pos/2 - 1] = temp;
        bubble_up (start_pos/2);
      }
  }

  void bubble_down (size_t start_pos = 1)   // receives 1−based indexing
  {
    if (start_pos*2 <= array.size ())
      {
        size_t theSmaller = start_pos*2;
        if (theSmaller + 1 <= array.size () &&
            array[theSmaller - 1] > array[theSmaller + 1 - 1]) theSmaller++;
        if (array[theSmaller - 1] < array[start_pos - 1])
          {
            sig temp = array[theSmaller - 1];
            array[theSmaller - 1] = array[start_pos - 1];
            array[start_pos - 1] = temp;
            bubble_down (theSmaller);
          }
      }
  }

  std::ostream& assertMinHeap (int j, std::ostream& o)
  {
    for (int i = 1; i <= array.size (); ++i)
      {
        if ((2*i     <= array.size () && array[i - 1] > array[2*i - 1]) ||
            (2*i + 1 <= array.size () && array[i - 1] > array[2*i + 1 - 1]))
          {
            o << " FAILED HEAP PROPERTY @: " << i << " WITH SIZE: " 
              << array.size () << std::endl;
            o << " called on position: " << j << std::endl;
            exit (0);
          } 
      }
    return o;
  }

  vector<sig> array;
  map<sig, J_Pair*> index;
};
#endif
