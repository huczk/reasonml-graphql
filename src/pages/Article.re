open ReactUtils;

[@react.component]
let make = (~slug) => {
  let (simple, _full) =
    ApolloHooks.useQuery(
      ~variables=Article_Query.GetArticle.makeVariables(~slug, ()),
      Article_Query.GetArticle.definition,
    );

  <div className="article-page">
    {switch (simple) {
     | Data(data) =>
       switch (data##article) {
       | Some(article) =>
         <>
           <div className="banner">
             <div className="container">
               <h1> {article##preview##title->s} </h1>
               <ArticleMeta article />
             </div>
           </div>
           <div className="container page">
             <div className="row article-content">
               <div
                 className="col-md-12"
                 dangerouslySetInnerHTML={
                   article##body |> Markdown.markdownToHtml |> dangerousHtml
                 }
               />
             </div>
             <hr />
             <div className="article-actions"> <ArticleMeta article /> </div>
             <div className="row">
               <div className="col-xs-12 col-md-8 offset-md-2">
                 <CommentAdd slug=article##preview##slug />
                 <CommentsList slug=article##preview##slug />
               </div>
             </div>
           </div>
         </>
       | None => React.null
       }
     | _ => React.null
     }}
  </div>;
};