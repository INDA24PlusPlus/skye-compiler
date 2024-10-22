let a<-0;
let b<-1;
let c<-10;
::loop;
let b<-b+a;
let a<-b-a;
print(a);
let c<-c-1;
jump_if(c>0) loop;
