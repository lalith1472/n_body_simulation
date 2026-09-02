#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <raymath.h>
#include <random>
#include <ctime>
#include <deque>
using namespace std;
#define G 100
const float density=10.0f;
bool pause=false;
const float pi=3.14f;


struct Body{

  Vector2 position;
  Vector2 velocity;
  float mass;
  deque<Vector2> trail;
  int radius;
  Color color;
  

};
struct Drag{
  
  bool start;
  bool end;
  float timer;
  Vector2 last;
  Vector2 old;
};

Drag Initialize_drag(){
  return (Drag){false,false,0.0f,{0,0},{0,0}};
  
}

void drag_velocity(Body &body, Drag &d,float t){
   
   if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
    
    Vector2 mouse=GetMousePosition();
    if(abs(mouse.x-body.position.x)<=body.radius && abs(mouse.y-body.position.y)<=body.radius){
      
      if(!(d.start)){
        d.start=true;
        d.old=body.position;
        
      }
      d.timer+=t;
      body.position=mouse;
    }
  }
  if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)&& d.start ){
    d.last=GetMousePosition();
    body.position=d.last;
    d.end=true;
  }
  if(d.start && d.end){
    body.velocity.x=(d.last.x-d.old.x)/d.timer;
    body.velocity.y=(d.last.y-d.old.y)/d.timer;
    d=Initialize_drag();
  }
}
// void set_color(Body &body){
//   Color color = {
//   (unsigned char)(rand() % 256),
//   (unsigned char)(rand() % 256),
//   (unsigned char)(rand() % 256),
//   255
//   };
//   body.color=color;

// }
void Drawbody(Body &body){
    

    DrawCircleV(body.position, body.radius,{ 0, 50, 150, 255 }); 
    cout << "weight: "<<body.mass<<" position "<<body.position.x<<" "<<body.position.y<<"\n";
}



float distance(Body &body1, Body &body2){

  float distance=sqrt(pow(body2.position.x-body1.position.x,2)+pow(body2.position.y-body1.position.y,2));
  return distance;
}

Vector2 force(Body &body1, Body &body2,float distance,bool &c){
  
  if(distance<=body1.radius+body2.radius){
    c=true;
    pause=false;
    return {0,0};
  }
  Vector2 unit={(body2.position.x - body1.position.x)/distance,(body2.position.y - body1.position.y)/distance};
  float force=(G * body1.mass * body2.mass) / pow(distance,2);
  Vector2 force_v={force*unit.x,force*unit.y};
  
  return force_v;
}

Vector2 acceleration(Body &body,Vector2 f){

  Vector2 a=Vector2Scale(f,1.0f/body.mass);
  return  a;
}

Vector2 velocity(Vector2 a,Body &body,float t){

  Vector2 velocity=Vector2Add(body.velocity,Vector2Scale(a,t));
  return velocity;
}

Vector2 position(Vector2 v,Body &body,float t){

  Vector2 position=Vector2Add(body.position,Vector2Scale(v,t));
  return position;
}

Body Initialize(){

  return {{0, 0}, {0, 0}, 1000, {},0};
}

int getradius(){

  float ran=(float)rand()/RAND_MAX;
  int radius;
  if(ran<=0.2){
    radius=rand()%11+20;
  }
  else {
    radius=rand()%5+1;
  }
  
  return radius;
}


void setup(vector<Body> &bodies){
  Vector2 pos=GetMousePosition();
  int colcheck=1;

  for(int i=0;i<bodies.size();i++){
    float dx=bodies[i].position.x-pos.x;
    float dy=bodies[i].position.y-pos.y;
    float d=sqrt(dx*dx+dy*dy);
    if((d<=bodies[i].radius+1)){
      colcheck=0;
      break;
    }
  }
   if(colcheck){
    Body body;
    body.position=GetMousePosition();
    body.radius=getradius();
    body.mass=(body.radius*body.radius*pi)*density;
    body.velocity={0,0};
    bodies.push_back(body);
   
   }
}


void physics(vector<Body> &bodies,bool &c,float t){
  vector<Vector2> accelerations;

  for(int i=0 ;i<bodies.size();i++){
    Vector2 net={0,0};
    for(int j=0;j<bodies.size();j++){
      if(i==j)continue;
      float d=distance(bodies[i],bodies[j]);
      
      Vector2 f=force(bodies[i],bodies[j],d,c);
      if(c)break;
      net+=f;
    }
    accelerations.push_back(acceleration(bodies[i],net));
    net={0,0};
  }
  for(int i=0;i<bodies.size();i++){
    bodies[i].velocity=velocity(accelerations[i],bodies[i],t);
    bodies[i].position=position(bodies[i].velocity,bodies[i],t);
  }
}
void maketrail(vector<Body> & bodies){
  int n=bodies.size();

  if(bodies.size()){
    for(int i=0;i<n;i++){
     
      bodies[i].trail.push_back(bodies[i].position);
      if(bodies[i].trail.size()>=50)bodies[i].trail.pop_front();
    }
  }
}
void printtrail(vector<Body> &bodies){
  for(int i=0;i<bodies.size();i++){
    for(int j=0;j<bodies[i].trail.size()-1;j++){
      Color color={ 0, 121, 241, (unsigned char)(j*255.0f/bodies[i].trail.size()-1)};
      DrawLineEx(bodies[i].trail[j],bodies[i].trail[j+1],bodies[i].radius*2,color);
    }
  }
}
int main(){
  
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 900, "2 Body Simulation");
  vector<Body> bodies;
  vector<Drag> dragging;
  
  Body body1 ,body2; 
  Drag drag1,drag2;
  
  srand(time(0));

  
  SetTargetFPS(100);
  bool collision=false;
  while (!WindowShouldClose())
  {   
  
    float t=GetFrameTime();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      setup(bodies);
      dragging.push_back(Initialize_drag());
    }
    for(int i=0;i<bodies.size();i++){
      drag_velocity(bodies[i],dragging[i],t);
    }
    if(IsKeyPressed(KEY_SPACE)){
      pause=!pause;
    }
    if(!pause && !collision){
      physics(bodies,collision,t);
    }
    if(IsKeyPressed(KEY_R)){
      bodies.clear();
      dragging.clear();
      collision=false;
    }
    maketrail(bodies);

    BeginDrawing();
    ClearBackground(WHITE);
    DrawText(TextFormat("FPS: %d",GetFPS()),0,0,20,WHITE);
    
    for(int i=0;i<bodies.size();i++){
      Drawbody(bodies[i]);
    }
    printtrail(bodies);

        // DrawRectangle(0,0,800,800,(Color){200,200,200,10});
        
    EndDrawing();
    
  }
    CloseWindow();
    return 0;
}