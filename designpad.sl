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
func pow(float j, int k) float{
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
func mod(int a, int b) int{
	while(a>=b){int a<-a-b;}
	return(a);
}
func main() int{
	for(int i<-1->1000:int b<-1){
		print((i+(int b<-i-b))-b);
	}
	print(pow(1.1,-4));
	return(0);
}
