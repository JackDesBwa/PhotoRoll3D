.pragma library

const names = [
    "Single view left",
    "Single view right",
    "Parallel view",
    "Cross view",
    "Parallel anamorphic view",
    "Cross anamorphic view",
    "Top-Bottom view",
    "Bottom-top view",
    "Top-Bottom anamorphic view",
    "Bottom-top anamorphic view",
    "Interleaved lines 1",
    "Interleaved lines 2",
    "Interleaved columns 1",
    "Interleaved columns 2",
    "Checkerboard 1",
    "Checkerboard 2",
    "Mirrored left",
    "Mirrored right",
    "Mirrored both",
    "Anaglyph Red/Cyan Gray",
    "Anaglyph Red/Cyan Half Colors",
    "Anaglyph Red/Cyan Full Colors",
    "Anaglyph Red/Cyan Dubois",
    "Anaglyph Yellow/Blue Gray",
    "Anaglyph Yellow/Blue Half Colors",
    "Anaglyph Yellow/Blue Full Colors",
    "Anaglyph Yellow/Blue Dubois",
    "Anaglyph Green/Magenta Gray",
    "Anaglyph Green/Magenta Half Colors",
    "Anaglyph Green/Magenta Full Colors",
    "Anaglyph Green/Magenta Dubois",
];

let ind = 0;
const SL=ind++, SR=ind++;
const P=ind++, X=ind++, PA=ind++, XA=ind++;
const TB=ind++, BT=ind++, TBA=ind++, BTA=ind++;
const IR1=ind++, IR2=ind++, IC1=ind++, IC2=ind++, CK1=ind++, CK2=ind++;
const ML=ind++, MR=ind++, MB=ind++;
const A_RC_G=ind++, A_RC_HC=ind++, A_RC_FC=ind++, A_RC_D=ind++;
const A_YB_G=ind++, A_YB_HC=ind++, A_YB_FC=ind++, A_YB_D=ind++;
const A_GM_G=ind++, A_GM_HC=ind++, A_GM_FC=ind++, A_GM_D=ind++;

function change(app, mode) {
    const n = names.length;
    while (mode < 0) mode += n;
    mode %= n;
    app.displayMode = mode;
}

function increment(app, inc=1) {
    change(app, app.displayMode + inc);
}
