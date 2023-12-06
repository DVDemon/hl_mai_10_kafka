#ifndef HTTPWEBSERVER_H
#define HTTPWEBSERVER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "http_request_factory.h"
#include "../config/config.h"
#include "../database/author.h"


class HTTPWebServer : public Poco::Util::ServerApplication
{

protected:
    int main([[maybe_unused]] const std::vector<std::string> &args)
    {
        try{
            database::Author::init();
            std::string format(
                config().getString("HTTPWebServer.format",
                                   DateTimeFormat::SORTABLE_FORMAT));
            
            ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", 8080));
            HTTPServer srv(new HTTPRequestFactory(format),
                           svs, new HTTPServerParams);
            srv.start();
            waitForTerminationRequest();
            srv.stop();
        }catch(std::exception &ex){
            std::cerr << "---------- exception: " << ex.what() << std::endl;
        }
        
        return Application::EXIT_OK;
    }

};
#endif // !HTTPWEBSERVER
