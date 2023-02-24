#include <Core/Transport/HTTPRequest/HTTPRequest.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <iostream>

namespace EFG
{
namespace Core
{
namespace Transport
{
      
      HTTPRequest::HTTPRequest(HTTPConfig _info) : info(_info) // pass refence 
      {
      
        Init();
        buildHeader();
        LoadGETRequestOptions();
        LoadPOSTRequestOptions();
        LoadPUTRequestOptions();
        LoadDELRequestOptions(); 
      }
      HTTPRequest::HTTPRequest(HTTPConfig _info, std::unordered_map<std::string, std::string> _header) : HTTPRequest(_info)
      {
        clientHeader = _header;
      }
      HTTPRequest::~HTTPRequest()
      {
        curl_easy_cleanup(GETHandler);
        curl_easy_cleanup(POSTHandler);
        curl_easy_cleanup(PUTHandler);
        curl_easy_cleanup(DELHandler);
      }
      void HTTPRequest::setHeader(std::unordered_map<std::string, std::string> header)
      {
        clientHeader = header;
      }
      void HTTPRequest::appendHeader(const std::string& key, const std::string& value)
      {
        clientHeader.insert({key, value});
      }
      std::string HTTPRequest::get(const std::string& path)
      {
         return get(path, {});
      }
      std::string HTTPRequest::get(const std::string& path, const std::unordered_map<std::string, std::string>& params)
      {
        std::string uri = info.getHttpEndPoint() + path + "?" + BuildQuery(params);
        ConfigureAdditionalGETRequestOptions(uri);
        std::string response="";
        curl_easy_setopt(GETHandler, CURLOPT_WRITEDATA, &response);
        CURLcode  statusCode = curl_easy_perform(GETHandler);
        if (statusCode != CURLE_OK) {
          std::cerr << "Error:: Request get, " 
                    << "Status Code: " << statusCode <<'\n';
        }
        return response;
      }
      std::string HTTPRequest::post(const std::string& path, const std::string& data)
      {

        std::string uri = info.getHttpEndPoint() + path;
        ConfigureAdditionalPOSTRequestOptions(uri, data);
        std::string response;
        curl_easy_setopt(POSTHandler, CURLOPT_WRITEDATA, &response);
        CURLcode  statusCode = curl_easy_perform(POSTHandler);
        if (statusCode != CURLE_OK) {
          std::cerr << "Error:: Request post, " 
                    << "Status Code: " << statusCode <<'\n'; // logger 
        }
        return response;
      }
      std::string HTTPRequest::put(const std::string& path, const std::string& data)
      {

        curl_easy_setopt(PUTHandler, CURLOPT_TIMEOUT, CONNECTION_TIMEOUT);
        curl_easy_setopt(PUTHandler, CURLOPT_WRITEFUNCTION, ConnectionWriteCallback);

        //curl_easy_setopt(PUTHandler, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(PUTHandler, CURLOPT_HTTPHEADER, curlHeader);
        curl_easy_setopt(PUTHandler, CURLOPT_CUSTOMREQUEST, "PUT");

        std::string uri = info.getHttpEndPoint() + path;
        
        curl_easy_setopt(PUTHandler, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(PUTHandler, CURLOPT_URL, uri.c_str());

        std::string response;
        curl_easy_setopt(PUTHandler, CURLOPT_WRITEDATA, &response);
        CURLcode  statusCode = curl_easy_perform(PUTHandler);
        if (statusCode != CURLE_OK) {
          std::cerr << "Error:: Request put, " 
                    << "Status Code: " << statusCode <<'\n'; // logger 
        }
        return response;
      }

      std::string HTTPRequest::del(const std::string& path, const std::string& data)
      {

        curl_easy_setopt(DELHandler, CURLOPT_TIMEOUT, CONNECTION_TIMEOUT);
        curl_easy_setopt(DELHandler, CURLOPT_WRITEFUNCTION, ConnectionWriteCallback);
        curl_easy_setopt(DELHandler, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(DELHandler, CURLOPT_HTTPHEADER, curlHeader);
        curl_easy_setopt(DELHandler, CURLOPT_CUSTOMREQUEST, "DELETE");

        std::string uri = info.getHttpEndPoint() + path;

        curl_easy_setopt(DELHandler, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(DELHandler, CURLOPT_URL, uri.c_str());

        std::string response;
        curl_easy_setopt(DELHandler, CURLOPT_WRITEDATA, &response);
        CURLcode  statusCode = curl_easy_perform(DELHandler);
        if (statusCode != CURLE_OK) {
          std::cerr << "Error:: Request del, " 
                    << "Status Code: " << statusCode <<'\n'; // logger 
        }
        return response;
      }
      void HTTPRequest::Init()
      {
        GETHandler = curl_easy_init();
        POSTHandler = curl_easy_init();
        PUTHandler = curl_easy_init();
        DELHandler = curl_easy_init();
        // other initialization

      }
      void HTTPRequest::LoadGETRequestOptions()
      {
        curl_easy_setopt(GETHandler, CURLOPT_TIMEOUT, CONNECTION_TIMEOUT);
        curl_easy_setopt(GETHandler, CURLOPT_VERBOSE, 0);
        curl_easy_setopt(GETHandler, CURLOPT_WRITEFUNCTION, ConnectionWriteCallback);
        curl_easy_setopt(GETHandler, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(GETHandler, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(GETHandler, CURLOPT_ACCEPT_ENCODING, "");

      }
      void HTTPRequest::LoadPOSTRequestOptions()
      {
        curl_easy_setopt(POSTHandler, CURLOPT_POST, 1);
        curl_easy_setopt(POSTHandler, CURLOPT_TIMEOUT, CONNECTION_TIMEOUT);
        curl_easy_setopt(POSTHandler, CURLOPT_VERBOSE, 0);
        curl_easy_setopt(POSTHandler, CURLOPT_WRITEFUNCTION, ConnectionWriteCallback);
        curl_easy_setopt(POSTHandler, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(POSTHandler, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(POSTHandler, CURLOPT_ACCEPT_ENCODING, "");

      }

      void HTTPRequest::LoadPUTRequestOptions()
      {
        curl_easy_setopt(PUTHandler, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(PUTHandler, CURLOPT_TCP_KEEPALIVE, 1L);

      }
      void HTTPRequest::LoadDELRequestOptions()
      {
        curl_easy_setopt(DELHandler, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(DELHandler, CURLOPT_TCP_KEEPALIVE, 1L);
      }
      void HTTPRequest::ConfigureAdditionalGETRequestOptions(const std::string& uri)
      {
        
        curl_easy_setopt(GETHandler, CURLOPT_HTTPHEADER, curlHeader); // move it the Common Options settings, if it doesn't change 
        curl_easy_setopt(GETHandler, CURLOPT_URL, uri.c_str());

      }
      void HTTPRequest::ConfigureAdditionalPOSTRequestOptions(const std::string& uri, const std::string& data)
      {
        curl_easy_setopt(POSTHandler, CURLOPT_HTTPHEADER, curlHeader);
        curl_easy_setopt(POSTHandler, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(POSTHandler, CURLOPT_URL, uri.c_str());

      }
      void HTTPRequest::ConfigureAdditionalPUTRequestOptions(const std::string& uri, const std::string& data)
      {

      }

      void HTTPRequest::ConfigureAdditionalDELRequestOptions(const std::string& uri, const std::string& data)
      {

      }
      void HTTPRequest::buildHeader() // build header evertime client side header setting is changed 
      {
        curlHeader = nullptr;
        for (auto it = clientHeader.begin(); it != clientHeader.end(); it++) {
          curlHeader = curl_slist_append(
            curlHeader,
            (it->first + ": " + it->second).c_str()
          );
        }
     }
     void HTTPRequest::clearHeader()
     {
       curlHeader = nullptr;
       clientHeader.clear(); 
     }


}
}
}
