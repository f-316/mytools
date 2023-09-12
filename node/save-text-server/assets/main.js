const app = document.querySelector('#App')
const div = document.createElement('div')
app.appendChild(div)

div.innerHTML = `
    <form>
        <div>
            <input type="submit" value="送信" formmethod="post" style="width: 100px; height: 80px; font-size: 20px;"/>
            <label for="save-text">テキストをサーバに保存します</label>
        </div>
        <div>
            <textarea id="save-text" name="save-text" style="width: ${window.innerWidth - 100}px; height: ${window.innerHeight - 100}px;"/></textarea>
        </div>
    </form>
`
