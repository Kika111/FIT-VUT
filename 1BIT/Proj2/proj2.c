/*
*	Name: IZP project 2
*	Autor: Kristína Hostačná
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


//https://www.rapidtables.com/math/algebra/Ln.html
//special cases logarithm
double _check_log(double x){
	//base<0
	if (x < 0.0) {
		//result=NAN
	    return NAN;
	}
	//base=1
	else if (x == 1.0){
	  	//result=0
	  	return 0.0;
	} 
	//base=0
	else if (x == 0.0) {
		//result= -∞
	    return -INFINITY;
	}
	//base= ∞
  	else if (isinf(x)){
  		//result= ∞
    	return INFINITY;
	}
	else{
		return 1;
	}
}

//function returning natural logarithm using combined fractions
//set number of iterations
double cfrac_log(double x, unsigned long n) {
	//check for special cases
	double check_log=1;
	check_log= _check_log(x);
	if (check_log != 1){
		return check_log;
	}
	double z;								
	double result=0, temp_result=1;			
	z= (x-1)/(x+1);			
	//cfrac logarithmic equation
	for (unsigned long i=n; 1 < i; i--){
		temp_result =((2*i)-1)-((i*i*z*z)/temp_result);
	}
	//special case with only one iteration
	if (n==1){
		result=(2*z)/temp_result;
	}
	//more than one iteration
	else {
		result=(2*z)/(1-(z*z/temp_result));
	}
	return result;
}

//function returning natural logarithm using Taylot algorithm
//set number of iterations
double taylor_log(double x, unsigned long n) {	
	//check for special cases
	double check_log=1;
	check_log= _check_log(x);
	if (check_log != 1){
		return check_log;
	}
	double result=0;
	n++;
	if (x<1){
		double y= 1-x;
		double  act_member=y;
		for (unsigned long i=1; i < n; i++){
			//add to sum
			result -= act_member;
			//taylor logarithmic equation for next polynomial member
			act_member= (act_member*y*i)/(i+1);
		}
		return result;
	}
	else{
		double first_member=(x-1)/x, act_member=first_member;
		for (unsigned long i=1; i <= n; i++){
			//add to sum
			result = result+ act_member;
			//taylor logarithmic equation for next polynomial member
			act_member= (act_member*first_member*i)/(i+1);
		}
		return result;
	}
}

//function returning natural logarithm 
//number of iterations decided in funct.
double mylog (double x){
	double check_log=1;
	check_log= _check_log(x);
	if (check_log != 1){
		return check_log;
	}
	double result=0;
	double temp_result=0;
	int i=1;
	//cycle measuring accuracy
	do{
		temp_result=result;
		//cfrac_log (cfrac_log is always more accurate)
		result=cfrac_log(x, i);
		i++;
	}while (fabs(temp_result-result)>= 0.000000001);
 	return result;
}

//https://www.rapidtables.com/math/number/exponent.html
//special cases power
double _check_pow(double x, double y){
	//exponent=0
	if (y == 0.0) {
	    //result=1
	    return 1;
	}
	//exponent=1
	else if (y == 1.0) {
	    //result=base
	    return x;
	}
	//base<0 
	else if (x < 0.0) {
		//result=NAN			
	    return NAN;
	}
	//base=0
	else if (x == 0.0){
		//result=0
	  	return 0.0;
	} 
	//base=1
	else if (x == 1.0){
	  	//result=1
	  	return 1.0;
	} 
	else{
		return -3;
	}
}

//crossroads power
//function returning x power y
//set number of iterations
double _pow(double x, double y, unsigned long n, int type){	
	//check for special cases
	double check_pow= _check_pow(x,y);
	if (check_pow != -3){
		return check_pow;
	}
	double result=1;
	double first_member;
	//pow -> taylor_pow
	if (type==1){
		first_member= y * taylor_log(x,n);
	}
	//pow -> taylorcf_pow
	else if (type==2){
		first_member= y * cfrac_log(x,n);
	}
	double act_member= first_member;
	for (unsigned long i=1; i <= n; i++){
		//add to sum
		result += act_member;
		//power equation for next polynomial member
		act_member= (act_member * first_member )/(i+1);
	}
	return result;
}

double taylor_pow(double x, double y, unsigned long n) {
	double result;
	result= _pow(x, y, n, 1);
	return result;
}

double taylorcf_pow(double x, double y, unsigned long n) {
	double result;
	result= _pow(x, y, n, 2);
	return result;
}

//function returning natural logarithm 
//number of iterations decided in funct.
double mypow(double x, double y){
	//check for special cases
	double check_pow= _check_pow(x,y);
	if (check_pow != -3){
		return check_pow;
	}
	double result=1, temp_result;
	//more accurate logarithm= more accurate power
	double first_member= y * mylog(x), act_member= first_member;
	int i=1;
	do{
		temp_result= result;
		result = result+ act_member;
		act_member= (act_member * first_member )/(i+1);
		i++;
	} while (fabs(temp_result-result)>= 0.000000001);
	return result;
}

int main(int argc, char *argv[]){
	//checking the number of arguments
	if (argc <= 3 || (strcmp(argv[1],"--log")=='\0' && argc >= 5) || (strcmp(argv[1],"--pow")=='\0' && argc >= 6)){
		printf ("Use:\nWrite '--log X N' or '--pow X Y N' in arguments\n");
		return 0;
	}
	//checking first argument
	else if (strcmp(argv[1],"--log")!=0 && strcmp(argv[1],"--pow")!=0){
		fprintf(stderr, "First argument format invalid.\n");
        return -1;  
	}
	else {
		//loading arguments
		double input[2]={'\0'};
		int N;
		for (int i=2; i< argc; i++){
			if (i < argc-1){
				input[i-2]= atof(argv[i]);
		        if (input == 0){
		            fprintf(stderr, "%d. argument must be a number.\n",i);
		            return -1;  
		        }
		    }
	    }
	    N= atol(argv[argc-1]);
	    //checking last argument
	    if (N <= 0){
	        fprintf(stderr, "%d. argument must be a positive number.\n",argc-1);
	        return -1;
	    }
		//logarithm results
	    if (strcmp(argv[1],"--log")=='\0'){
	    	double result_log, result_cf, result_t, result_m;
	    	result_log= log(input[0]);
	    	result_cf = cfrac_log(input[0],N);
	    	result_t = taylor_log(input[0], N);
	    	result_m = mylog(input[0]);
	    	printf ("       log(%.4g) = %.12g\n",input[0], result_log);
	    	printf (" cfrac_log(%.4g) = %.12g\n", input[0], result_cf);
	    	printf ("taylor_log(%.4g) = %.12g\n", input[0], result_t);
	    	printf ("     mylog(%.4g) = %.7e\n", input[0], result_m);
	    }
	    //power results
	    else if (strcmp(argv[1],"--pow")=='\0'){
	    	double result_pow, result_cf, result_t, result_m;
	    	result_pow= pow(input[0],input[1]);
	    	result_t = taylor_pow(input[0], input[1], N);
	    	result_cf = taylorcf_pow(input[0], input[1],N);
	    	result_m = mypow(input[0],input[1]);
	    	printf("         pow(%.4g,%.4g) = %.12g\n", input[0], input[1], result_pow);
	    	printf("  taylor_pow(%.4g,%.4g) = %.12g\n", input[0], input[1], result_t);
	    	printf("taylorcf_pow(%.4g,%.4g) = %.12g\n", input[0], input[1], result_cf);
	    	printf("       mypow(%.4g,%.4g) = %.7e\n", input[0], input[1], result_m);
	    }
	}
	return 0;
}


