function login()
{
    var temp = Qt.createQmlObject('Login{anchors.fill: parent}',mainArea);
    temp.login.connect(function(nickname,groupname){

    }
    )
    temp.cancle.connect(function(){
        Qt.quit()
    }
    )
}
