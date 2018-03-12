
circle_radius = 10;
space = 3;
border = 5;



class Matrix
{
    constructor(x, y)
    {
        this.div = document.getElementById("matrix")
        this.size = x * y;
        this.cur_x = border + circle_radius;
        this.cur_y = border + circle_radius;
        this.x = x;
        this.y = y;
        this.width = circle_radius*x*2 + space*x - space + border*2 - 1;
        this.height = circle_radius*y*2 + space*y - space + border*2 - 1;

        this.leds = new Array();

        this.paper = Raphael(this.div, this.width + 2, this.height + 2);
        this.rect = this.paper.rect(1, 1, this.width, this.height);
        this.rect.attr({stroke: "black", "stroke-width": "5"});
        

        for(let i=1; i<=this.y; i++)
        {
            for(let i=1; i<=this.x; i++)
            {
                this.leds.push(new Led(this.cur_x, this.cur_y, this.paper));
                this.cur_x += circle_radius*2 + space;
            }
            this.cur_y += (circle_radius*2 + space);
            this.cur_x = (border + circle_radius);   
        }
        //alert(this.leds[54].state)

    }

    get data()
    {
        let txt = new String();
        var tmp;
        for (var i = 0; i<=this.size - 1; i++)
        {   
            

            tmp = (this.leds[i].state);
            txt += tmp;
            console.log(txt);
        }
        return txt;
    }
}


class Led
{
    constructor(x, y, screen)
    { 
        this.pressed = false;
        var c = screen.circle(x, y, circle_radius);
        c.attr("fill", "white");
        var self = this;

        c[0].onclick = function () 
        {
            self.pressed = !self.pressed;

            if (self.pressed){
                c.attr("fill", "red");  
            }
            else{c.attr("fill", "white"); } 
        }
    }

    get state()
    {
        if (this.pressed){return "1";}
        else{return "0";}
    }
}