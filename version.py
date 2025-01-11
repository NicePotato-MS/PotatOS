import json
import sys

# A little messy, need to redo this with a better build system

def update_version(version_file, version_type=None):
    with open(version_file, 'r') as f:
        version = json.load(f)
    
    if version_type == "major":
        version["major"] += 1
        version["minor"] = 0
        version["patch"] = 0
        version["build"] = 0
    elif version_type == "minor":
        version["minor"] += 1
        version["patch"] = 0
        version["build"] = 0
    elif version_type == "patch":
        version["patch"] += 1
        version["build"] = 0
    else:
        version["build"] += 1

    with open(version_file, 'w') as f:
        json.dump(version, f, indent=2)
    
    print(f"ver {version['major']} {version['minor']} {version['patch']} {version['build']}")

if __name__ == "__main__":
    version_file = sys.argv[1]
    version_type = sys.argv[2] if len(sys.argv) > 2 else None
    update_version(version_file, version_type)
