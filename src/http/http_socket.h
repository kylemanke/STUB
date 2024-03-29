#ifndef STUB_HTTP_HTTP_SOCKET_H
#define STUB_HTTP_HTTP_SOCKET_H

#include "types.h"
#include "socket/socket.h"

#include "http_defines.h"
#include "http_status_codes.h"

// TODO: Create the HTTPSocket class
// Responsible for receiving and sending HTTP messages over a single socket connection
// Will buffer in socket read to avoid small reads
// Will either read in a HTTP message or send one out
// Will provide interfaces to create the HTTPResponse or HTTPRequest
// Will provide the necessary interface for future HTTPServer and HTTPSession class
// May eventually provide HTTPS support

class HTTPSocket
{
public:
    /**
     * TORS
    */
    
    /// @brief Constructor to create a blank HTTPSocket
    HTTPSocket();
    
    /// @brief Constructor to create HTTPSocket off of a current Socket
    /// @param sock Desired Socket
    HTTPSocket(Socket sock);

    /// @brief Constructor to create HTTPSocket off of a host and port
    /// @param host Desired hostname to connect to
    /// @param port Desired port to connect to
    HTTPSocket(const char *host, const char *port);

    /// @brief Destructor to clean up buffer
    ~HTTPSocket();

    /**
     * SOCKET FUNCTIONS
    */
    
    /// @brief Set a new socket, will shutdown current socket if exists
    /// @param sock Desired socket
    void SetSocket(Socket sock);
    
    /// @brief Set a new socket, will shutdown current socket if exists
    /// @param host 
    /// @param port 
    void SetSocket(const char *host, const char *port);
    
    /// @brief Check current socket is valid
    /// @return Will return true if valid
    bool Valid();
    
    /// @brief Shutdown the current socket
    void Shutdown();

    /// @brief Close the current socket
    void Close();

    /**
     * HTTP FUNCTIONS
    */

    /// @brief Check for pending bytes in the socket
    /// @return True if bytes are pending
    bool PendingBytes();
    
    /// @brief Send the completed HTTPRequest message
    /// @param request 
    void SendRequest(HTTPRequest& request);

    /// @brief Send the completed HTTPResponse message
    /// @param response 
    void SendResponse(HTTPResponse& response);
    
    /// @brief Parse a Request Message from the socket
    /// @param request 
    void FetchNextRequest(HTTPRequest& request);

    /// @brief Parse a response message from the socket
    /// @param response 
    void FetchNextResponse(HTTPResponse& response);

private:
    /**
     * DATA MEMBERS
    */
    Socket socket_;
    char *buffer_;
    uint32 idx_;
    uint32 buffer_size_;
    uint32 buffer_max_;

    /**
     * PARSING FUNCTIONS
    */

    /// @brief Parse the HTTP Method in the start line
    /// @return HTTPMethod enum value
    HTTPMethod ParseHTTPMethod();

    /// @brief Parse the request target in the start line
    /// @return URL of the request target
    std::string ParseRequestTarget();

    /// @brief Parse the HTTP Version
    /// @return HTTPVersion enum value
    HTTPVersion ParseHTTPVersion();

    /// @brief Parse the HTTP Status code
    /// @return Return the HTTPStatusCode enum value
    HTTPStatusCode ParseStatus();

    /// @brief Parse the next header
    /// @return (hdr, hdr_str) or (,) if blank line
    std::pair<std::string, std::string> ParseNextHeader();


    /// @brief Fetch the http body as a string
    /// @param body_size Size of the HTTP body
    /// @return String containing the HTTP body
    std::string ParseBody(uint32 body_size);

    /// @brief Fetch the next byte from the buffer
    /// @return Return the next byte, will throw HTTPReadError if nothing to be read
    char NextByte();

    /// @brief Will peek the next byte without incrementing the buffer
    /// @return Next byte in the buffer
    char PeekByte();
    
    /// @brief Fetch the next size bytes from the buffer
    /// @param dest Pointer to destination buffer of at least size
    /// @param size Size to read in to dest
    /// @return Number of bytes read, will throw HTTPReadError if nothing to be read
    uint32 NextBytes(char *dest, uint32 size);
};

#endif