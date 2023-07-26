# FunCoding

This is a repository for storing my daily coding practice. I use this repository to motivate myself to write code every day and to track my progress over time.

- zh_CN [简体中文](/README.zh_CN.md)


```html
<div class="directory">
  <ul>
    <li><span class="toggle">+</span> Algorithm
      <ul class="nested">
        <li>File 1</li>
        <li>File 2</li>
      </ul>
    </li>
    <li><span class="toggle">+</span> Folder B
      <ul class="nested">
        <li><span class="toggle">+</span> Subfolder B1
          <ul class="nested">
            <li>File 3</li>
            <li>File 4</li>
          </ul>
        </li>
        <li>File 5</li>
      </ul>
    </li>
    <li>File 6</li>
  </ul>
</div>

<style>
  .directory ul {
    list-style: none;
    padding-left: 20px;
  }
  .directory .toggle {
    cursor: pointer;
    margin-right: 5px;
  }
  .directory .nested {
    display: none;
  }
  .directory .active {
    display: block;
  }
</style>

<script>
  var toggles = document.querySelectorAll(".toggle");
  for (var i = 0; i < toggles.length; i++) {
    toggles[i].addEventListener("click", function() {
      this.parentElement.querySelector(".nested").classList.toggle("active");
      this.textContent = this.textContent == "+" ? "-" : "+";
    });
  }
</script>
