open ReactUtils;

[@react.component]
let make = (~article) => {
  <div className="article-preview">
    <div className="article-meta">
      <Author
        username=article##author##username
        image=article##author##image
        createdAt=article##createdAt
      />
      <FavoriteButton
        id=article##id
        slug=article##slug
        count=article##favoritesCount
        favorited=article##favorited
        small=true
      />
    </div>
    <Link href={"#/article/" ++ article##slug} className="preview-link">
      <h1> {article##title->s} </h1>
      <p> {article##description->s} </p>
      <span> "Read more..."->s </span>
      <TagsOutline tagList=article##tagList />
    </Link>
  </div>;
};