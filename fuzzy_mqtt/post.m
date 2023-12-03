
api_key = 'NXGPB1CDKCPB9H9U';  
data = 20; 


url = 'https://api.thingspeak.com/update.json'; 


data = struct('api_key', api_key, 'field1', num2str(data)); 

options = weboptions('MediaType', 'application/json');  
response = webwrite(url, data, options);

if strcmp(response, '0')
    disp('Data not posted to ThingSpeak. Check your API key and data.');
else
    disp('Data posted to ThingSpeak successfully.');
end
