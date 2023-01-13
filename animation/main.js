const CIRCLE = 0;
const SQUARE = 1;

class NetCanvas {
    constructor() {
        this.ws = null;
        this.connected = false;
        // Figure out a good resolution
        let res = 0.8*Math.min(window.innerWidth, window.innerHeight);
        this.res = res;
        const canvas = document.getElementById("canvas");
        canvas.style.width = res;
        canvas.style.height = res;
        // Add an svg element with this resolution
        this.svg = d3.select("#canvas")
                    .append("svg")
                    .attr("width", res)
                    .attr("height", res);
        // Add reference to log DOM element
        this.log = document.getElementById("log");
    }

    /**
     * Connect to an address
     * 
     * @param {string} addr Address:port to which to connect
     */
    connect(addr) {
        const that = this;
        this.ws = new WebSocket(addr);
        this.ws.addEventListener("error", (event) => {
            that.log.innerHTML = "<span style=\"color:red;\">ERROR: Could not connect to " + event.target.url + "</span>";
        });
        this.ws.binaryType = 'arraybuffer';
        this.ws.onopen = this.onopen.bind(this);
        this.ws.onmessage = this.onmessage.bind(this);
        this.ws.onclose = this.onmessage.bind(this);
    }

    /**
     * Callback for when a connection is successfully open
     * @param {object} event Event
     */
    onopen(event) {
        this.log.innerHTML = "<span style=\"color:green;\">Successfully connected to " + event.target.url + "!</span>";
        this.ws.send("draw");
    }

    /**
     * Callback for when a message is received 
     * @param {object} evt Event
     */
    onmessage(evt) {
        let fbuf = new Float32Array(evt.data);
        let bbuf = new Uint8Array(evt.data);
        let i = 0;
        let circles = [];
        while (i < bbuf.length) {
            let r = bbuf[i+1];
            let g = bbuf[i+2];
            let b = bbuf[i+3];
            if (bbuf[i] == CIRCLE) {
                i += 4;
                let circle = {"fill":"rgb("+r+","+g+","+b+")"};
                circle.cx = fbuf[i/4]*this.res;
                i += 4;
                circle.cy = fbuf[i/4]*this.res;
                i += 4;
                circle.r = fbuf[i/4]*this.res;
                i += 4;
                circles.push(circle);
            }
        }
        this.repaint({"circles":circles});
    }

    onclose(evt) {

    }

    /**
     * Clear the svg and draw a bunch of new shapes
     * @param {object} shapes Shapes with field "circles"
     */
    repaint(shapes) {
        const svg = this.svg;
        svg.selectAll('*').remove();

        // Draw circles
        if (shapes.circles.length > 0) {
            let circles = svg.selectAll(null)
            .data(shapes.circles)
            .enter()
            .append("circle");
            for (const [key, value] of Object.entries(shapes.circles[0])) {
                circles.attr(key, d=>d[key]);
            }
        }
        this.ws.send("draw");
    }
}