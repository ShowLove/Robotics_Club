data=load("velocity.log");
n=size(data,1);


%subplot(3,1,1);
%plot(data([10:n],1),data([10:n],8),data([10:n],1),data([10:n],11))
%subplot(3,1,2);
%plot(data([10:n],1),data([10:n],9),data([10:n],1),data([10:n],12))
%subplot(3,1,3);
%plot(data([10:n],1),data([10:n],10),data([10:n],1),data([10:n],13))

subplot(2,1,1);
plot(data([10:n],1),data([10:n],8),data([10:n],1),data([10:n],11))
subplot(2,1,2);
plot(data([10:n],1),data([10:n],14))
