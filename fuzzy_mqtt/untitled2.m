api_key_read = 'NXGPB1CDKCPB9H9U';
channel_ID = '2327722';

url = sprintf('https://api.thingspeak.com/channels/%s/fields/1/last.json?api_key=%s', channel_ID, api_key_read);

options = weboptions('RequestMethod', 'get');
response = webread(url, options);

value = response.field1;
disp(value);
