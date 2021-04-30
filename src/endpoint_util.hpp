// endpoint_util.hpp
// Ashish D'Souza

#ifndef ENDPOINT_UTIL_HPP
#define ENDPOINT_UTIL_HPP

#define CORS(responseWriter) responseWriter.headers().add<Http::Header::AccessControlAllowOrigin>("*")

#endif
