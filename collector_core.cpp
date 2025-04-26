#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;

void splitPath(const fs::path& p, vector<string>& parts) {
    for (auto e : p) {
        parts.push_back(e.string());
    }
}

string makeNewName(string fn, int cnt) {
    auto pos = fn.rfind('.');
    if (pos == string::npos) {
        return fn + "_" + to_string(cnt);
    } else {
        return fn.substr(0, pos) + "_" + to_string(cnt) + fn.substr(pos);
    }
}

void copyFiles(fs::path cur, fs::path baseIn, fs::path baseOut,
                     int lim, int depth,
                     unordered_map<string, int>& used) {

    if (!fs::exists(cur)) return;

    for (auto it : fs::directory_iterator(cur)) {
        fs::path relAll = fs::relative(it.path(), baseIn);
        fs::path relDir = relAll.parent_path();

        if (fs::is_regular_file(it.path())) {
            string fn = it.path().filename().string();
            if (!fn.empty() && fn[0]=='.') continue;

            vector<string> segs;
            splitPath(relDir, segs);

            if (lim != -1) {
                while ((int)segs.size() > lim - 1) {
                    segs.erase(segs.begin());
                }
            }

            fs::path dest = baseOut;
            for (auto& s : segs) dest /= s;

            if (!fs::exists(dest)) {
                fs::create_directories(dest);
            }

            string check = (dest / fn).string();
            string outName = fn;
            if (used.count(check)) {
                int c = ++used[check];
                outName = makeNewName(fn,c);
            } else {
                used[check] = 0;
            }

            fs::copy_file(it.path(), dest / outName, fs::copy_options::overwrite_existing);
        }
        else if (fs::is_directory(it.path())) {
            copyFiles(it.path(), baseIn, baseOut, lim, depth+1, used);
        }
    }
}

int main(int argc, char* argv[]) {
    fs::path inP = argv[1], outP = argv[2];
    int limit = -1;
    if (argc==4) {
        limit = stoi(argv[3]);
    }

    if (!fs::exists(outP)) {
        fs::create_directories(outP);
    }

    unordered_map<string,int> usedMap;
    copyFiles(inP, inP, outP, limit, 1, usedMap);
    return 0;
}
