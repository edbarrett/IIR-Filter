%Elijah Barrett
%ECE 4020
%IIR Filter Laboratory
%This code is created to produce a and b values for the Direct Form I
%Structure of an IIR Filter. 
%4/14/2018

fs = 16000;
fn = fs/2;
w1 = 1032.4/fn;
w2 = 1167.6/fn;
Wp = [w1 w2]; %Digital filtr so 

format long
[b,a] = cheby1(2, 1.5, Wp); %Return a and b

%Splitting the filter into two sections
format long
[sos,g] = tf2sos(b,a);

%Writing out the coefficients
a=[sos(1,4) sos(1,5) sos(1,6); sos(2,4) sos(2,5) sos(2,6)];
b=[sos(1,1) sos(1,2) sos(1,3);sos(2,1) sos(2,2) sos(2,3)];

%Dividing by 4
a=a/4
b=b/4

