let markdownToHtml = markdown =>
  MarkdownIt.render(MarkdownIt.createMarkdownIt(), markdown);