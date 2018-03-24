

function send_request(server){
    request = new XMLHttpRequest();
    request.open("GET", server, true);
    request.send();
}

function send_img(data){
    let server = "/img?img="+data;
    send_request(server);
}