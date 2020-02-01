open ReactUtils;
open BsReform;

module StateLenses = [%lenses
  type state = {
    title: string,
    description: string,
    body: string,
    tagList: array(string),
  }
];

module ArticleForm = ReForm.Make(StateLenses);

let useArticleForm = (~onSubmit) => {
  ArticleForm.use(
    ~schema={
      ArticleForm.Validation.Schema([|
        Custom(
          Description,
          values =>
            Js.String.length(values.description) == 0
              ? Error("Description must not be empty") : Valid,
        ),
        Custom(
          Body,
          values =>
            Js.String.length(values.body) == 0
              ? Error("Body must not be empty") : Valid,
        ),
        Custom(
          Title,
          values =>
            Js.String.length(values.title) == 0
              ? Error("Title must not be empty") : Valid,
        ),
      |]);
    },
    ~onSubmit=
      ({state}) => {
        onSubmit(state.values);

        None;
      },
    ~initialState={title: "", description: "", body: "", tagList: [||]},
    (),
  );
};

let tagDelimitr = ", ";
let tagDecode = tagsStr => tagsStr |> Js.String.split(tagDelimitr);
let tagEncode = tags => tags |> Js.Array.joinWith(tagDelimitr);
let goToAdricle = slug => ReasonReactRouter.push("/#/article/" ++ slug);
let getUpdateSlug =
    (response: ApolloHooks.Mutation.result(Article_Query.UpdateArticle.t)) =>
  switch (response) {
  | Data(article) =>
    switch (article##updateArticle) {
    | Some(article) => article##slug
    | _ => ""
    }
  | _ => ""
  };
let getAddSlug =
    (response: ApolloHooks.Mutation.result(Article_Query.AddArticle.t)) =>
  switch (response) {
  | Data(article) =>
    switch (article##addArticle) {
    | Some(article) => article##slug
    | _ => ""
    }
  | _ => ""
  };

[@react.component]
let make = (~slug) => {
  let editing =
    switch (slug) {
    | None => false
    | _ => true
    };

  let (simple, _full) =
    ApolloHooks.useQuery(
      ~skip=!editing,
      ~variables=
        Article_Query.GetArticle.makeVariables(
          ~slug=Belt.Option.getWithDefault(slug, ""),
          (),
        ),
      Article_Query.GetArticle.definition,
    );

  let (mutationAdd, simpleAdd, _full) =
    ApolloHooks.useMutation(Article_Query.AddArticle.definition);

  let (mutationUpdate, simpleEditing, _full) =
    ApolloHooks.useMutation(Article_Query.UpdateArticle.definition);

  let {state, submit, handleChange, setValues}: ArticleForm.api =
    useArticleForm(~onSubmit=({title, description, body, tagList}) =>
      if (editing) {
        mutationUpdate(
          ~variables=
            Article_Query.UpdateArticle.makeVariables(
              ~slug=Belt.Option.getWithDefault(slug, ""),
              ~title,
              ~description,
              ~body,
              (),
            ),
          (),
        )
        |> Js.Promise.then_(r =>
             r |> getUpdateSlug |> goToAdricle |> Js.Promise.resolve
           )
        |> ignore;
      } else {
        mutationAdd(
          ~variables=
            Article_Query.AddArticle.makeVariables(
              ~title,
              ~description,
              ~body,
              ~tagList,
              (),
            ),
          (),
        )
        |> Js.Promise.then_(r =>
             r |> getAddSlug |> goToAdricle |> Js.Promise.resolve
           )
        |> ignore;
      }
    );

  React.useEffect1(
    () => {
      switch (simple) {
      | Data(data) =>
        switch (data##article) {
        | Some(a) =>
          setValues({
            title: a##preview##title,
            description: a##preview##description,
            body: a##body,
            tagList: a##preview##tagList->Belt.Array.keepMap(x => x),
          });
          None;
        | None => None
        }
      | _ => None
      }
    },
    [|simple|],
  );

  <>
    <ul className="error-messages">
      {editing
         ? Errors.getGqlErrors(simpleEditing)
         : Errors.getGqlErrors(simpleAdd)}
      {Errors.getFormErrors(state.fieldsState)}
    </ul>
    <form onSubmit={eventPrevent(_ => submit())}>
      <fieldset>
        <fieldset className="form-group">
          <input
            type_="text"
            className="form-control form-control-lg"
            placeholder="Article Title"
            value={state.values.title}
            onChange={handleFieldChange(handleChange(Title))}
          />
        </fieldset>
        <fieldset className="form-group">
          <input
            type_="text"
            className="form-control"
            placeholder="What's this article about?"
            value={state.values.description}
            onChange={handleFieldChange(handleChange(Description))}
          />
        </fieldset>
        <fieldset className="form-group">
          <textarea
            className="form-control"
            rows=8
            placeholder="Write your article (in markdown)"
            value={state.values.body}
            onChange={handleFieldChange(handleChange(Body))}
          />
        </fieldset>
        {editing
           ? React.null
           : <fieldset className="form-group">
               <input
                 type_="tags"
                 className="form-control"
                 placeholder="Enter tags"
                 value={state.values.tagList |> tagEncode}
                 onChange={handleFieldChange(v =>
                   v |> tagDecode |> handleChange(TagList)
                 )}
               />
             </fieldset>}
        <button className="btn btn-lg pull-xs-right btn-primary">
          "Publish Article"->s
        </button>
      </fieldset>
    </form>
  </>;
};