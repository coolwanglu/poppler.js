function post_image(filename) {
    var data = FS.root.contents[filename].contents;
    data = data.map(function(x){ return String.fromCharCode(unSign(x,8)); }).join('');
    var img = document.createElement('img');
    img.src = 'data:image/png;base64,' + btoa(data);
    document.getElementById('container').appendChild(img);
}
post_image('out-1.png');
