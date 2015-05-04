
var HelloWorldLayer = cc.Layer.extend(
{
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();

        /////////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.
        // ask the window size
        var size = cc.winSize;

        var btn = new ccui.Button();
        btn.setTouchEnabled(true);
        btn.setPressedActionEnabled(true);
        btn.loadTextures
        (res.HelloWorld_png,
        res.HelloWorld_png,
        "");
                                      
        btn.attr({
           x: size.width / 2,
           y: size.height / 2,
           scale: 0.5,
           rotation: 0
           });

        var selector = function(touch, event) {
            cc.log(event);
            switch (event)
            {
            case ccui.Widget.TOUCH_ENDED:
          {
              cc.log("touched");
              var test = ns.TestObj.create();
                                      test.retain();
              test.functionTest("functionTest", function(ret) { cc.log(ret);
                                test.release();});
          }
            
            break;
            default:
            break;
            }

            return true;
        }
                                      
        btn.addTouchEventListener(selector, this);
        this.addChild(btn);
        return true;
    }
});



var HelloWorldScene = cc.Scene.extend(
                                      {
                                      onEnter:function () {
                                      this._super();
                                      var layer = new HelloWorldLayer();
                                      this.addChild(layer);
                                      }
                                      });

