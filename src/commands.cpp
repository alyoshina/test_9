#include "commands.h"

void ICommands::execInAnyCase() {
    if (!cmds.empty()) {
        auto it = cmds.begin();
        std::shared_ptr data = std::make_shared<Data>(std::format("bulk{}", (*it)->getCreateTimeSec()),
                                                        "bulk: " + (*it)->getName());
        std::for_each(std::next(cmds.begin()), cmds.end(), [this, &data] (auto c) {
            data->data += ", " + c->getName();
        });
        data->data += '\n';

        std::for_each(oList.begin(), oList.end(), [&data] (auto l) {
            l->addData(data);
            // l->endl();
            // l->postsetting();
        });
        cmds.clear();
    }
}

// void ICommands::execInAnyCase() {
//     if (!cmds.empty()) {
//         auto it = cmds.begin();
//         std::for_each(oList.begin(), oList.end(), [&it] (auto l) {
//             l->setName(std::format("bulk{}.log", (*it)->getCreateTimeSec()));
//             if (l->presetting()) {
//                 *l << "bulk: " << (*it)->getName();
//             }
//         });
//         std::for_each(std::next(cmds.begin()), cmds.end(), [this] (auto c) {
//             std::for_each(oList.begin(), oList.end(), [&c] (auto l) { *l << ", " << c->getName(); });
//         });
//         std::for_each(oList.begin(), oList.end(), [] (auto l) {
//             l->endl();
//             l->postsetting();
//         });

//         cmds.clear();
//     }
// }