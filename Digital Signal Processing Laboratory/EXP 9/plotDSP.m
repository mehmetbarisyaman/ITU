original = csvread('C:\Users\LabPC\Desktop\original.dat');
reel = csvread('C:\Users\LabPC\Desktop\reel.dat');
imaginer = csvread('C:\Users\LabPC\Desktop\imaginer.dat');

obtained = zeros(0, 700);

for i=1:700
    obtained(i) = reel(i) -1i*imaginer(i);
end

fft_signal = fft(original);


figure
subplot(2,3,1)
plot(original)
title('Original Signal')

subplot(2,3,2)
plot(abs(obtained))
title('Obtained Signal')

subplot(2,3,3)
plot(reel)
title('Reel')

subplot(2,3,4)
plot(imaginer)
title('Imaginer')

subplot(2,3,5)
plot(fft_signal)
title('FFT Signal')