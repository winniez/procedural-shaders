//
//  Object class
//  Dummy generic object class
//  Location of object is common to all objects
//  Translation is defined for all objects, but must
//  be implemented in the display function of each object
//  The display function is virtual and must be reimplemented
//
#ifndef OBJECT_H
#define OBJECT_H

class Object
{
protected:
   float x0,y0,z0;                          //  Location
   Object(float x=0,float y=0,float z=0);   //  Constructor
public:
   void translate(float x,float y,float z); //  Set translation
   virtual void display()=0;                //  Render the object
   virtual ~Object() {};                    //  Do nothing destructor
};

#endif
