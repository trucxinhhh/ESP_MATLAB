c=4;
for s=1:c
    api_key_read = 'NXGPB1CDKCPB9H9U';
    channel_ID = '2327722';
    url_post='https://api.thingspeak.com/update.json'; 

    %lấy dữ liệu
    data = thingSpeakRead(2327722,'Fields',1,NumPoints=1);
    display(data);
    %xử lý mờ
    
    fisData = getFISCodeGenerationData('Fuzzy_Model.fis');
    output = evalfis(fisData,data);
    disp(output);
    %đóng gói dữ liệu trước khi post
    data1 = struct('api_key', api_key, 'field2', num2str(output));
    send_data=jsonencode(data1);
    disp(send_data)
    %gửi dữ liệu
    options_post = weboptions('MediaType', 'application/json');  
    response_post = webwrite(url_post, send_data, options_post);
    pause(2)

end