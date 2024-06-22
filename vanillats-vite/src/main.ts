import './styles/style.css';
import { TheHeader } from './components/TheHeader.ts';
import { TheContents } from './components/TheContents.ts';

document.querySelector<HTMLDivElement>('#app')!.innerHTML = `
  <div id="header"></div>
  <div id="contents"></div>
  <div id="footer"></div>
`;

const theHeader = new TheHeader();
document.querySelector('#header')!.append(theHeader);

const theContents = new TheContents();
document.querySelector('#contents')!.append(theContents);

setTimeout(() => {
  theHeader.backgroundColor = 'tomato';
}, 2000);
