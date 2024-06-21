import './style.css';
import { MyButton } from './MyButton.ts';
import typescriptLogo from './typescript.svg';
import viteLogo from '/vite.svg';
import { setupCounter } from './counter.ts';

document.querySelector<HTMLDivElement>('#app')!.innerHTML = `
  <div>
    <a href="https://vitejs.dev" target="_blank">
      <img src="${viteLogo}" class="logo" alt="Vite logo" />
    </a>
    <a href="https://www.typescriptlang.org/" target="_blank">
      <img src="${typescriptLogo}" class="logo vanilla" alt="TypeScript logo" />
    </a>
    <h1>Vite + TypeScript</h1>
    <div class="card">
      <button id="counter" type="button"></button>
    </div>
    <!-- case 1 -->
    <my-button>
      ボタン
    </my-button>
    <!-- case 2 -->
    <div id="my-button">
    </div>
    <p class="read-the-docs">
      Click on the Vite and TypeScript logos to learn more
    </p>
  </div>
`;

const colors = ['red', 'blue', 'green'];
setupCounter(document.querySelector<HTMLButtonElement>('#counter')!);

// case 1
customElements.define('my-button', MyButton);

// case 2
const myButton = new MyButton();
myButton.setAttribute('text', 'hoge');
myButton.setAttribute('color', 'blue');
const myButtonTemp = document.querySelector('#my-button');
myButtonTemp?.append(myButton);

let index = 0;
setInterval(() => {
  myButton.setAttribute('color', colors[index++ % colors.length]);
  myButton.setAttribute('text', `count: ${index}`);
}, 1000);
