import { EL, SVG } from "@alexgyver/component";
import DragBlock from "@alexgyver/drag-block";
import { constrain, DelaySender, fetchT, map, waitFrame } from "@alexgyver/utils";
import './index.css'

let joy = {};
let sender = new DelaySender(async v => {
    await fetchT(window.location.origin + '/xy?' + new URLSearchParams({ x: v.x, y: v.y }).toString(), {
        timeout: 1000,
    });
    console.log(v);

}, 50);

function render() {
    let s = joy.$svg.clientWidth;
    if (!s) return;
    SVG.config(joy.$joy, { attrs: { cx: s / 2, cy: s / 2, r: s / 4 } });
    SVG.config(joy.$ring, { attrs: { cx: s / 2, cy: s / 2, r: s / 2 - 5 } });
}

document.addEventListener("DOMContentLoaded", () => {
    EL.context = joy;
    EL.make('div', {
        parent: document.body,
        style: 'flex-shrink:1;display:flex;justify-content:center;',
        child: EL.make('svg', {
            style: 'width:100%;aspect-ratio:1/1;max-width:300px',
            $: 'svg',
            children: [
                SVG.circle(0, 0, 0, { style: 'fill:none;stroke:green' }, { $: 'ring' }),
                SVG.circle(0, 0, 0, { fill: 'green', filter: 'drop-shadow(2px 2px 3px rgba(0, 0, 0, 0.2))' }, { class: 'joy', $: 'joy' }),
            ],
        })
    });

    render();
    let resizer = new ResizeObserver(() => waitFrame().then(render));
    resizer.observe(joy.$svg);

    DragBlock(joy.$joy, e => {
        let s = joy.$svg.clientWidth;
        if (e.touch && e.type == 'move') e.type = 'drag';

        switch (e.type) {
            case 'press':
            case 'tpress':
                SVG.config(joy.$ring, { style: 'stroke-width: 2' });
                break;

            case 'drag':
            case 'tdrag':
                const attr = v => Number(joy.$joy.getAttribute(v));
                let r = attr('r');
                let offs = r + 6;
                let x = constrain(s / 2 + e.drag.x, offs, s - offs);
                let y = constrain(s / 2 + e.drag.y, offs, s - offs);

                SVG.config(joy.$joy, { attrs: { cx: x, cy: y } });

                let sx = map(x, offs, s - offs, -255, 255) | 0;
                let sy = map(y, offs, s - offs, 255, -255) | 0;
                sender.send({ x: sx, y: sy });
                break;

            case 'release':
            case 'trelease':
                SVG.config(joy.$ring, { style: 'stroke-width: 1' });
                render();
                sender.send({ x: 0, y: 0 });
                break;
        }
    });
});