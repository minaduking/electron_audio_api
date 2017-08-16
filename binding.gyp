{
    "targets": [{
        "target_name": "electron_audio_api",
        "sources": [
            "src/main.cc",
            "src/audio.cc",
        ],
        "include_dirs": ["<!(node -e \"require('nan')\")"],
        "conditions": [
            [
                "OS=='linux'",
                {}
            ],
            [
                "OS=='win'",
                {
                    "sources": [
                        "lib/win/CAA_audio.cc",
                        "lib/win/CAA_audio_object.cc",
                    ],
                    "cflags": ["-Wall", "-std=c++11"],
                    "cflags_cc!": ["-fbi-rtti", "-fno-exceptions"],
                    "cflags!": ["-fno-exceptions"],
                    "include_dirs": [],
                    "libraries": [],
                    "defineed": [
                        "NTDDI_WIN10"
                    ]
                }
            ],
            [
                "OS=='mac'",
                {
                    "sources": [
                        "lib/mac/CAA_audio_object.mm"
                    ],
                    "defines": [
                        '__MACOSX_CORE__'
                    ],
                    "link_settings": {
                        'libraries': [
                            '-framework Foundation',
                            '-framework CoreAudio',
                        ]
                    },
                    "xcode_settings": {
                        "GCC_ENABLE_CPP_EXCEPTIONS": 'YES',
                        "CLANG_CXX_LIBRARY": 'libc++',
                        "OTHER_CPLUSPLUSFLAGS": ["-std=c++11","-stdlib=libc++", "-v"],
                        "OTHER_LDFLAGS": ["-stdlib=libc++"],
                        "MACOSX_DEPLOYMENT_TARGET": "10.7"
                    }
                }
            ]
        ]
    }]
}
