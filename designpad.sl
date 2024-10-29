func fib(int q) int{
	if(q>2){
		int o<-fib(q-1)+fib(q-2);	
	}
	else{
		int o<-1;
	}
	return(o);
}
func fact(int n) int{
	int retVal<-1;
	for(int i<-1->(n+1):1){
		int retVal<-retVal*i;
	}
	return(retVal);
}
func factf(float n) float{
	if(n<1.0){
		float r=1.0;
	}
	else{
		float r=factf(n-1.0);
		float r=r*n;
	}
	return(r);
}
func exp(float j, int k) float{
	float reV=1.0;
	int divC=0;
	if(k<0){
		int k=-k;
		int divC=1;
	}
	else{1==1;}
	for(int i<-0->k:1){
		float reV=reV*j;
	}
	if(divC>0){
		float reV=1.0/reV;
	}else{1==1;}
	return(reV);
}
func sine(float y) float{	
	float iF<-3.0;
	float r<-y;
	int a<-1;
	for(int i<-3->23:2){
		float r<-r+exp(-1.0,a)*(exp(y,i)/factf(iF));
		float iF<-iF+2.0;
		int a<-a+1;
	}
	return(r);
}
func cosine(float y) float{
	float iF<-2.0;
	float r<-1.0;
	int a<-1;
	for(int i<-2->20:2){
		float r<-r+exp(-1.0,a)*(exp(y,i)/factf(iF));
		float iF<-iF+2.0;
		int a<-a+1;
	}
	return(r);
}
func eexp(float x) float{
	float iF<-2.0;
	float r<-1.0+x;
	for(int i<-2->10:1){
		float r<-r+exp(x,i)/factf(iF);
		float iF<-iF+1.0;
	}
	return(r);
}
func modf(float fa, float fb) float{
	while(fa>=fb){float fa<-fa-fb;}
	return(fa);
}
func mod(int a, int b) int{
	while(a>=b){int a<-a-b;}
	return(a);
}
func tang(float xy) float{
	return(sine(xy)/cosine(xy));
}
func tangsq(float xy) float{
	return(tang(xy)*tang(xy));
}
func main() int{
	for(int i<-1->1000:int b<-1){
		print((i+(int b<-i-b))-b);
	}
	print(tang(2.0));
	return(0);
}
