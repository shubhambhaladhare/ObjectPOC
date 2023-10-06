#pragma once

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include "publisher.hpp"

Pistache::Rest::Route::Result handleOperation(publisher* pub, const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
void sendoperation(publisher* pub, std::string operation, std::string suboperation, std::string accountName, std::string password);
