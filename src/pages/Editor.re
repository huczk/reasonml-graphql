[@react.component]
let make = (~slug) => {
  <div className="editor-page">
    <div className="container page">
      <div className="row">
        <div className="col-md-10 offset-md-1 col-xs-12">
          <ArticleForm slug />
        </div>
      </div>
    </div>
  </div>;
};