
data = thingSpeakRead(2327722,'Fields',1,NumPoints=1);
display(data);
fisData = getFISCodeGenerationData('Fuzzy_Model.fis');
output = evalfis(fisData,data);
disp(output);
thingSpeakWrite(2327722,'Fields',2,'Values',output,'WriteKey','NXGPB1CDKCPB9H9U')
pause(10)
