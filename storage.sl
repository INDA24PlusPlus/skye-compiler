func tang(float x) float{
	float fa<-sine(x);
	print(x);
	float fb<-cosine(x);
	float retr<-0.0;
	if(fb!=0.0){
		float retr<-fa/fb;
	}
	else {
		float retr<-0.0;
	}
	return(retr);
}
