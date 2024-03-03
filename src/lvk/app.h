#ifndef LVK_APP_H
#define LVK_APP_H
#pragma once

namespace lvk {

    class app {

    public:
        virtual signed run() = 0;

        virtual ~app() = default;
    };

}

#endif //LVK_APP_H
