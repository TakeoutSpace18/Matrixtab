void FSSD_init(void){
  

  HTTP.on("/list", HTTP_GET, printDirectory);
  HTTP.on("/edit", HTTP_DELETE, handleDelete);
  HTTP.on("/edit", HTTP_PUT, handleCreate);
  HTTP.on("/edit", HTTP_POST, [](){ returnOK(); }, handleFileUpload);
  HTTP.onNotFound(handleNotFound);

 

  if (SD.begin(SS)){
     Serial.println("SD Card initialized.");
     hasSD = true;
  }
}

void returnOK() {
  HTTP.send(200, "text/plain", "");
}

void returnFail(String msg) {
  HTTP.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";

  File dataFile = SD.open(path.c_str());
  if(dataFile.isDirectory()){
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (HTTP.hasArg("download")) dataType = "application/octet-stream";

  if (HTTP.streamFile(dataFile, dataType) != dataFile.size()) {
    
  }

  dataFile.close();
  return true;
}

void handleFileUpload(){
  if(HTTP.uri() != "/edit") return;
  HTTPUpload& upload = HTTP.upload();
  if(upload.status == UPLOAD_FILE_START){
    if(SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    
  } else if(upload.status == UPLOAD_FILE_END){
    if(uploadFile) uploadFile.close();
    
  }
}

void deleteRecursive(String path){
  File file = SD.open((char *)path.c_str());
  if(!file.isDirectory()){
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while(true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" +entry.name();
    if(entry.isDirectory()){
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete(){
  if(HTTP.args() == 0) return returnFail("BAD ARGS");
  String path = HTTP.arg(0);
  if(path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate(){
  if(HTTP.args() == 0) return returnFail("BAD ARGS");
  String path = HTTP.arg(0);
  if(path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if(path.indexOf('.') > 0){
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if(file){
      file.write((const char *)0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory() {
  if(!HTTP.hasArg("dir")) return returnFail("BAD ARGS");
  String path = HTTP.arg("dir");
  if(path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  HTTP.setContentLength(CONTENT_LENGTH_UNKNOWN);
  HTTP.send(200, "text/json", "");
  WiFiClient client = HTTP.client();

  HTTP.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
    break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    HTTP.sendContent(output);
    entry.close();
 }
 HTTP.sendContent("]");
 dir.close();
}

void handleNotFound(){
  if(hasSD && loadFromSdCard(HTTP.uri())) return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += HTTP.uri();
  message += "\nMethod: ";
  message += (HTTP.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += HTTP.args();
  message += "\n";
  for (uint8_t i=0; i<HTTP.args(); i++){
    message += " NAME:"+HTTP.argName(i) + "\n VALUE:" + HTTP.arg(i) + "\n";
  }
  HTTP.send(404, "text/plain", message);
  
}


