/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "spleeter/logging/logging.h"

namespace spleeter
{
namespace logging
{
LoggingWrapper::LoggingWrapper(const LogSeverity& severity) : LoggingWrapper{severity, true} {}

LoggingWrapper::LoggingWrapper(const LogSeverity& severity, const bool should_log)
    : severity_{severity}, should_log_{should_log}
{
}

std::stringstream& LoggingWrapper::Stream() { return stream_; }

LoggingWrapper::~LoggingWrapper()
{
    if (should_log_)
    {
        switch (severity_)
        {
#ifndef NDEBUG
            case LogSeverity::DEBUG:
#endif
            case LogSeverity::INFO:
            case LogSeverity::WARN:
                std::cout << stream_.str() << std::endl;
                break;
            case LogSeverity::ERROR:
                std::cerr << stream_.str() << std::endl;
                break;
            case LogSeverity::FATAL:
                std::cerr << stream_.str() << std::endl;
                std::flush(std::cerr);
                std::abort();
                break;
            default:
                break;
        }
    }
}
}  // namespace logging
}  // namespace spleeter